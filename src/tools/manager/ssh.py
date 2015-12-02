#coding: utf-8
import paramiko

def sshConnect(ipaddr, port, usr, psd):
    print("ssh.Connect: %s:%d %s" %(ipaddr, port, usr))
    conn = paramiko.SSHClient()
    conn.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    conn.connect(ipaddr, port, usr, psd)
    return conn

def sshClose(conn):
    conn.close()

def doCommand(conn, cmd):
    print("ssh.doCommand: %s" %(cmd))
    stdin, stdout, stderr = conn.exec_command(cmd)
    ret = stdout.read()
    print ret
    return ret

def sftpConnect(ipaddr, port, usr, psd):
    print("ssh.sftpConnect: %s:%d %s" %(ipaddr, port, usr))
    conn = paramiko.Transport((ipaddr, port))
    conn.connect(username=usr, password=psd)
    return conn

def sftpClose(conn):
    conn.close()

def upload(conn, local, remote):
    print("ssh.upload: %s -> %s" %(local, remote))
    sftp = paramiko.SFTPClient.from_transport(conn)
    sftp.put(local, remote)

def download(conn, local, remote):
    print("ssh.download: %s -> %s" %(remote, local))
    sftp = paramiko.SFTPClient.from_transport(conn)
    sftp.get(remote, local)
