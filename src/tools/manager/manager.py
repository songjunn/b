#coding: utf-8
import sys,os
import time, datetime
import commands
import ssh
import tools
import servers

CONFIG_PATH = "./config/%d.lua"
SERVERS_FILE = "./config/servers.json"
MONGO_INIT_FILE = "./config/mongo_gamedb_init.js"
PACK_PATH = "./version/"
MONGO_PATH = "../../external/mongo/bin/"
SERVER_PATH = "../../../../exec/"
SERVER_NAME = "server"
SERVER_ARCHIVE = "odin"
UPLOAD_ROOT = "/root/odin/"
UPLOAD_PATH = UPLOAD_ROOT + "%d/"


def help():
    print "manager command:"
    print "deploy                  install and init server"
    print "start   [servers]       start the servers"
    print "        [login]         start the loginserver"
    print "        [gmweb]         start the gmweb"
    print "stop    [servers]       stop the servers"
    print "        [login]         stop the loginserver"
    print "        [gmweb]         stop the gmweb"
    print "state   [servers]       show the server status" 
    print "        [login]         show the loginserver status"
    print "        [gmweb]         show the gmweb status"
    print "pack    [servers]       pack to version/odin-version.tar.gz. Not include sconf.lua."
    print "        [scripts]       pack to version/odin-scripts-version.tar.gz"
    print "        [data]          pack to version/odin-data-version.tar.gz"
    print "        [gmweb]         pack to version/gmweb-version.tar.gz"
    print "update  [servers]       publish new version (include odin***tar.gz, odin***tar.gz.md5, sconf.lua) to server"
    print "        [scripts]       update lua scripts to server"
    print "        [data]          update csv data to server"
    print "        [sconf]         update sconf.lua to server"
    print "        [gmweb]         update gmweb to server"
    print "reload  [scripts]       reload lua script"
    print "        [data]          reload csv data"
    print "mongo   [pack]          pack mongo db bin"
    print "        [install]       install mongo db"
    print "        [init]          initlize mongo db"
    print "        [start]         start mongo db"
    print "        [backup]        backup mongo db"
    print "        [restore]       restore mongo db"

########################################################################################################################
## base functions
def shell(id, cmd):
    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    conn = ssh.sshConnect(ip, port, usr, psd)
    output = ssh.doCommand(conn, cmd)
    ssh.sshClose(conn)
    return output

def pack(name, path):
    filename = name + "-" + time.strftime('%Y%m%d-%H%M%S') + ".tar.gz"
    filepath = os.path.abspath(PACK_PATH) + "/" + filename
    os.system("cd %s; tar czvf %s %s --exclude='*.svn' --exclude='log/*' --exclude='monitor/*' --exclude='core.*' --exclude='*.out' --exclude='*~' --exclude='sconf.lua'" %(SERVER_PATH, filepath, path))
    os.system("cd %s; md5sum %s > %s" %(PACK_PATH, filename, filename + ".md5"))

def publish(id, format, upPath):
    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    conn = ssh.sftpConnect(ip, port, usr, psd)

    cmd = "cd %s; ls -lt %s | awk '{print $9}' | sed -n '1p;'" %(PACK_PATH, format)
    (status, packFile) = commands.getstatusoutput(cmd)
    packMd5 = packFile + ".md5"
    
    print("uploading zip file ...")
    ssh.upload(conn, os.path.abspath(PACK_PATH + packFile), upPath + packFile)

    print("uploading md5 file ...")
    ssh.upload(conn, os.path.abspath(PACK_PATH + packMd5), upPath + packMd5)

    print("checking md5 ...")
    cmd = "cd %s; md5sum -c %s" %(upPath, upPath + packMd5)
    shell(id, cmd)

    print("unziping file ...")
    cmd = "cd %s; tar -zxvf %s" %(upPath, upPath + packFile)
    shell(id, cmd)

    ssh.sftpClose(conn)


########################################################################################################################
## install functions
def deploy(id):
    cmd = "mkdir odin; mkdir odin/%d" %(id)
    shell(id, cmd)

    updateServers(id, "")

    cmd = "cd odin/%d/server; chmod 777 *server; chmod 777 *.py; chmod 777 *.sh" %(id)
    shell(id, cmd)


########################################################################################################################
## start functions
def start(id):
    path = UPLOAD_PATH %(id) + SERVER_NAME
    cmd = "cd %s; chmod 777 *.sh; ./start.sh" %(path)
    shell(id, cmd)

def startLogin(id):
    path = UPLOAD_PATH %(id) + SERVER_NAME
    cmd = "cd %s; chmod 777 *.sh; ./start-login.sh" %(path)
    shell(id, cmd)

def startGmweb(id):
    shell(id, "cd %s/GMWeb; nohup python GMWeb.py 8081 > out.log 2>&1 &" %(UPLOAD_ROOT))


########################################################################################################################
## stop functions
def stop(id):
    path = UPLOAD_PATH %(id) + SERVER_NAME
    cmd = "cd %s; chmod 777 *.sh; ./stop.sh" %(path)
    shell(id, cmd)

def stopLogin(id):
    path = UPLOAD_PATH %(id) + SERVER_NAME
    cmd = "cd %s; chmod 777 *.sh; ./stop.sh" %(path)
    shell(id, cmd)

def stopGmweb(id):
    shell(id, "ps -axu | grep GMWeb.py | awk '{print $2}' |xargs kill -9")


########################################################################################################################
## state functions
def state(id):
    cmd = "ps aux | grep server | grep -v grep"
    shell(id, cmd)

def stateLogin(id):
    cmd = "ps aux | grep loginserver | grep -v grep"
    shell(id, cmd)

def stateGmweb(id):
    cmd = "ps aux | grep GMWeb.py | grep -v grep"
    shell(id, cmd)


########################################################################################################################
## pack functions
def packServers():
    pack(SERVER_ARCHIVE, "server")

def packScripts():
    pack("scripts", "server/scripts")

def packData():
    pack("data", "server/data")

def packGMWeb():
    filename = "gmweb" + "-" + time.strftime('%Y%m%d-%H%M%S') + ".tar.gz"
    filepath = os.path.abspath(PACK_PATH) + "/" + filename
    os.system("cd %s; tar czvf %s %s --exclude='*.svn' --exclude='*.pyc'" %("../Web", filepath, "GMWeb"))
    os.system("cd %s; md5sum %s > %s" %(PACK_PATH, filename, filename + ".md5"))


########################################################################################################################
## update functions
def updateServers(id, pack):
    if pack == "":
        publish(id, "odin*.tar.gz", UPLOAD_PATH %(id))
    else:
        publish(id, pack, UPLOAD_PATH %(id))

    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    conn = ssh.sftpConnect(ip, port, usr, psd)

    print("uploading sconf file ...")
    ssh.upload(conn, os.path.abspath(CONFIG_PATH %(id)), UPLOAD_PATH %(id) + "server/sconf.lua")
    ssh.sftpClose(conn)

def updateSconf(id):
    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    conn = ssh.sftpConnect(ip, port, usr, psd)

    print("uploading sconf file ...")
    ssh.upload(conn, os.path.abspath(CONFIG_PATH %(id)), UPLOAD_PATH %(id) + "server/sconf.lua")
    ssh.sftpClose(conn)

def updateScripts(id):
    publish(id, "scripts*.tar.gz", UPLOAD_PATH %(id))

def updateData(id):
    publish(id, "data*.tar.gz", UPLOAD_PATH %(id))

def updateGMWeb(id):
    publish(id, "gmweb*.tar.gz", UPLOAD_ROOT)
    stopGmweb(id)
    startGmweb(id)


########################################################################################################################
## reload functions
def reloadScripts(id):
    cmd = "ps aux | grep gameserver | grep -v grep | awk '{print $2}'"
    pid = shell(id, cmd)

    cmd = "kill -s USR2 %s" %(pid)
    shell(id, cmd)

def reloadData(id):
    cmd = "ps aux | grep gameserver | grep -v grep | awk '{print $2}'"
    pid = shell(id, cmd)
    
    cmd = "kill -s RTMIN %s" %(pid)
    shell(id, cmd)


########################################################################################################################
## mongo functions
def packMongo():
    zipfile = "%s/mongodb.zip" %(PACK_PATH)
    tools.zip_dir(MONGO_PATH, zipfile)
    os.system("cd %s; md5sum mongodb.zip > mongodb.md5" %(PACK_PATH))

def installMongo(id):
    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    conn = ssh.sftpConnect(ip, port, usr, psd)

    print("uploading zip file ...")
    ssh.upload(conn, os.path.abspath(PACK_PATH + "/mongodb.zip"), UPLOAD_ROOT+"/mongodb.zip")

    print("uploading md5 file ...")
    ssh.upload(conn, os.path.abspath(PACK_PATH + "/mongodb.md5"), UPLOAD_ROOT+"/mongodb.md5")

    print("checking md5 ...")
    cmd = "cd %s; md5sum -c %s" %(UPLOAD_ROOT, "./mongodb.md5")
    shell(id, cmd)

    print("creating mongodb folder ...")
    cmd = "mkdir %s/mongodb; mkdir %s/mongodb/bin; mkdir %s/mongodb/data; mkdir %s/mongodb/log; touch %s/mongodb/log/mongodb.log" %(UPLOAD_ROOT, UPLOAD_ROOT, UPLOAD_ROOT, UPLOAD_ROOT, UPLOAD_ROOT)
    shell(id, cmd)

    print("unziping file ...")
    cmd = "cd %s; unzip -o -d ./mongodb/bin %s" %(UPLOAD_ROOT, "./mongodb.zip")
    shell(id, cmd)

    startMongo(id)

    ssh.sftpClose(conn)

def initMongo(id):
    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    conn = ssh.sftpConnect(ip, port, usr, psd)

    print("uploading mongo init file ...")
    ssh.upload(conn, os.path.abspath(MONGO_INIT_FILE), UPLOAD_ROOT + os.path.basename(MONGO_INIT_FILE))

    cmd = "cd %s/mongodb/bin; ./mongo odin_gamedb_%d '%s'" %(UPLOAD_ROOT, id, UPLOAD_ROOT + os.path.basename(MONGO_INIT_FILE))
    shell(id, cmd)
    ssh.sftpClose(conn)

def startMongo(id):
    print("starting mongo ...")
    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    cmd = "cd %s/mongodb/bin; chmod 777 *; ./mongod --dbpath=../data/ --logpath=../log/mongodb.log --logappend --smallfiles --profile=1 --slowms=10&" %(UPLOAD_ROOT)
    shell(id, cmd)

def exportMongoDb(id):
    print("export mongodb ...")
    ip, port, usr, psd = servers.getServer(id, servers.loadData(SERVERS_FILE))
    cmd = "~/odin/mongodb/bin/mongodump -h %s -d odin_gamedb_%d -o ./dbdump" %(ip, id)
    print(cmd)
    (status, output) = commands.getstatusoutput(cmd)
    print status, output

def importMongoDb(id):
    print("import mongodb ...")
    cmd = "~/odin/mongodb/bin/mongorestore -d backup_odin_gamedb_%d ./dbdump/odin_gamedb_%d" %(id, id)
    (status, output) = commands.getstatusoutput(cmd)
    print status, output


########################################################################################################################
## main function
def main():
    if len(sys.argv) == 1:
        help()
    elif sys.argv[1] == "shell":
        shell(int(sys.argv[2]), sys.argv[3])
    elif sys.argv[1] == "deploy":
        deploy(int(sys.argv[2]))

    elif sys.argv[1] == "start" and sys.argv[2] == "servers":
        start(int(sys.argv[3]))
    elif sys.argv[1] == "start" and sys.argv[2] == "login":
        startLogin(int(sys.argv[3]))
    elif sys.argv[1] == "start" and sys.argv[2] == "gmweb":
        startGmweb(int(sys.argv[3]))

    elif sys.argv[1] == "stop" and sys.argv[2] == "servers":
        stop(int(sys.argv[3]))
    elif sys.argv[1] == "stop" and sys.argv[2] == "login":
        stopLogin(int(sys.argv[3]))
    elif sys.argv[1] == "stop" and sys.argv[2] == "gmweb":
        stopGmweb(int(sys.argv[3]))

    elif sys.argv[1] == "state" and sys.argv[2] == "servers":
        state(int(sys.argv[3]))
    elif sys.argv[1] == "state" and sys.argv[2] == "login":
        stateLogin(int(sys.argv[3]))
    elif sys.argv[1] == "state" and sys.argv[2] == "gmweb":
        stateGmweb(int(sys.argv[3]))
    
    elif sys.argv[1] == "pack" and sys.argv[2] == "servers":
        packServers()
    elif sys.argv[1] == "pack" and sys.argv[2] == "scripts":
        packScripts()
    elif sys.argv[1] == "pack" and sys.argv[2] == "data":
        packData()
    elif sys.argv[1] == "pack" and sys.argv[2] == "gmweb":
        packGMWeb()

    elif sys.argv[1] == "mongo" and sys.argv[2] == "pack":
        packMongo()
    elif sys.argv[1] == "mongo" and sys.argv[2] == "init":
        initMongo(int(sys.argv[3]))
    elif sys.argv[1] == "mongo" and sys.argv[2] == "install":
        installMongo(int(sys.argv[3]))
    elif sys.argv[1] == "mongo" and sys.argv[2] == "start":
        startMongo(int(sys.argv[3]))
    elif sys.argv[1] == "mongo" and sys.argv[2] == "backup":
        exportMongoDb(int(sys.argv[3]))
    elif sys.argv[1] == "mongo" and sys.argv[2] == "restore":
        importMongoDb(int(sys.argv[3]))

    elif sys.argv[1] == "update" and sys.argv[2] == "servers":
        if len(sys.argv) == 4:
            updateServers(int(sys.argv[3]), "")
        elif len(sys.argv) == 5:
            updateServers(int(sys.argv[3]), sys.argv[4])
    elif sys.argv[1] == "update" and sys.argv[2] == "sconf":
        updateSconf(int(sys.argv[3]))
    elif sys.argv[1] == "update" and sys.argv[2] == "scripts":
        updateScripts(int(sys.argv[3]))
    elif sys.argv[1] == "update" and sys.argv[2] == "gmweb":
        updateGMWeb(int(sys.argv[3]))
    elif sys.argv[1] == "update" and sys.argv[2] == "data":
        updateData(int(sys.argv[3]))

    elif sys.argv[1] == "reload" and sys.argv[2] == "scripts":
        reloadScripts(int(sys.argv[3]))
    elif sys.argv[1] == "reload" and sys.argv[2] == "data":
        reloadData(int(sys.argv[3]))

    else:
        print("invalid command")

if __name__ == "__main__":
    main()
