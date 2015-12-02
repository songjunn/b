import web
import datetime
import time
import pymongo
import string
import GMWeb
import logging
from pymongo import *

logging.basicConfig(level=logging.DEBUG,
                format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                datefmt='%a, %d %b %Y %H:%M:%S',
                filename='log/myapp.log',
                filemode='w')

#################################################################################################
console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)
#################################################################################################


format_date = "%Y-%m-%d"
format_datetime = "%Y-%m-%d %H:%M:%S"
conn = pymongo.MongoClient('localhost', 27017)
db = conn.odin_gamedb_024

def connectDB(serverID):
    global conn
    global db
    if(0 != cmp("0", serverID)):
#    print GMWeb.confDic[serverID]
       intPort = string.atoi(GMWeb.confDic[serverID][3][1])
#    print GMWeb.confDic[serverID][2][1]
#       print intPort
       database = GMWeb.confDic[serverID][1][1]
#       print database
       conn = pymongo.MongoClient(GMWeb.confDic[serverID][2][1], intPort)
       print conn
#       print GMWeb.confDic[serverID][2][1]
       db = conn[database]
       print db

def connectServer0DB():
     global conn
     global db
     intPort = string.atoi(GMWeb.serverInfo0Dic["serverInfo0"][3][1])
#     print intPort
     database = GMWeb.serverInfo0Dic["serverInfo0"][1][1]
#     print database
     conn = pymongo.MongoClient(GMWeb.serverInfo0Dic["serverInfo0"][2][1], intPort)
#     print GMWeb.serverInfo0Dic["serverInfo0"][2][1]
     db = conn[database]

def logInfoConnectDB(serverID):
    global logInfoConn
    global logInfoDb
    if(0 != cmp("0", serverID)):
#    print GMWeb.confDic[serverID]
       intPort = string.atoi(GMWeb.confDic[serverID][3][1])
#    print GMWeb.confDic[serverID][2][1]
#       print intPort
       database = GMWeb.confDic[serverID][1][1]
#       print database
       logInfoConn = pymongo.MongoClient(GMWeb.confDic[serverID][2][1], intPort)
       print logInfoConn
#       print GMWeb.confDic[serverID][2][1]
       logInfoDb = logInfoConn[database]
       print logInfoDb

def logInfoConnectServer0DB():
     global logInfoConn0
     global logInfoDb0
     intPort = string.atoi(GMWeb.serverInfo0Dic["serverInfo0"][3][1])
#     print intPort
     database = GMWeb.serverInfo0Dic["serverInfo0"][1][1]
#     print database
     logInfoConn0 = pymongo.MongoClient(GMWeb.serverInfo0Dic["serverInfo0"][2][1], intPort)
#     print GMWeb.serverInfo0Dic["serverInfo0"][2][1]
     logInfoDb0 = logInfoConn0[database]

def saveConnectDB(serverID):
    global saveConn
    global saveDb
    if(0 != cmp("0", serverID)):
#    print GMWeb.confDic[serverID]
       intPort = string.atoi(GMWeb.confDic[serverID][3][1])
#    print GMWeb.confDic[serverID][2][1]
#       print intPort
       database = GMWeb.confDic[serverID][1][1]
#       print database
       saveConn = pymongo.MongoClient(GMWeb.confDic[serverID][2][1], intPort)
       print saveConn
#       print GMWeb.confDic[serverID][2][1]
       saveDb = saveConn[database]
       print saveDb

def saveConnectServer0DB():
     global saveConn0
     global saveDb0
     intPort = string.atoi(GMWeb.serverInfo0Dic["serverInfo0"][3][1])
#     print intPort
     database = GMWeb.serverInfo0Dic["serverInfo0"][1][1]
#     print database
     saveConn0 = pymongo.MongoClient(GMWeb.serverInfo0Dic["serverInfo0"][2][1], intPort)
#     print GMWeb.serverInfo0Dic["serverInfo0"][2][1]
     saveDb0 = saveConn0[database]

def rechargeConnectDB(serverID):
    global rechargeConn
    global rechargeDb
    if(0 != cmp("0", serverID)):
#    print GMWeb.confDic[serverID]
       intPort = string.atoi(GMWeb.confDic[serverID][3][1])
#    print GMWeb.confDic[serverID][2][1]
#       print intPort
       database = GMWeb.confDic[serverID][1][1]
#       print database
       rechargeConn = pymongo.MongoClient(GMWeb.confDic[serverID][2][1], intPort)
       print rechargeConn
#       print GMWeb.confDic[serverID][2][1]
       rechargeDb = rechargeConn[database]
       print rechargeDb

def rechargeConnectServer0DB():
     global rechargeConn0
     global rechargeDb0
     intPort = string.atoi(GMWeb.serverInfo0Dic["serverInfo0"][3][1])
#     print intPort
     database = GMWeb.serverInfo0Dic["serverInfo0"][1][1]
#     print database
     rechargeConn0 = pymongo.MongoClient(GMWeb.serverInfo0Dic["serverInfo0"][2][1], intPort)
#     print GMWeb.serverInfo0Dic["serverInfo0"][2][1]
     rechargeDb0 = rechargeConn0[database]

def get_posts():
    return db.entries.find()

def get_PlayerInfos(serverID):
#    intPort = string.atoi(GMWeb.confDic[serverID][3][1])
#    conn = pymongo.MongoClient(GMWeb.confDic[serverID][2][1], intPort)
#    db = conn.GMWeb.confDic[serverID][1][1]

    connectDB(serverID)
#    for i in db.player.find({},{"playerid":1, "name":1, "level":1, "fighting":1}):
#        print i['name']
    return db.player.find({},{"playerid":1, "name":1, "level":1, "fighting":1})

def get_PlayerInfo(selectType, playerValue, serverID):
    result = 0
    connectDB(serverID)
    intType = string.atoi(selectType)
    if(0 == intType):
        intPlayerID = string.atoi(playerValue)
        result = db.player.find({"playerid":intPlayerID})
    elif(1 == intType):
        result = db.player.find({"name":playerValue})
    print result[0]['playerid']
    return result

def get_OnlinePlayer(serverID):
    connectDB(serverID)
    result = 0
    return result

def get_CurrItem(selectType, playerValue, serverID):
    result = 0
    connectDB(serverID)
    intType = string.atoi(selectType)
    if(0 == intType):
        intPlayerID = string.atoi(playerValue)
        result = db.item.find({"playerid":intPlayerID})
    elif(1 == intType):
        result = db.item.find({"name":playerValue})
    return result

def get_MyHeroes(selectType, playerValue, serverID):
    result = 0
    connectDB(serverID)
    intType = string.atoi(selectType)
    if(0 == intType):
        intPlayerID = string.atoi(playerValue)
        result = db.hero.find({"playerid":intPlayerID})
    elif(1 == intType):
        result = db.hero.find({"name":playerValue})
    return result

def get_ServerInfo(secs):
    logging.info('getServerInfo')
    connectServer0DB()
    serverInfos = db.serverInfo.find()
    for serverInfo in serverInfos:
        GMWeb.serverInfoDic[serverInfo['_id']] = serverInfo

def getDateElements(sdate):
    dformat = format_date
    sdate = time.strptime(sdate, dformat)
    return sdate

def getCurrDate():
     currDate = datetime.datetime.now()
     stringCurrDate = currDate.strftime("%Y-%m-%d")
     return stringCurrDate
#    return time.strftime(format_date, time.localtime(time.time()))

def getCurrentDateX():
    return time.strftime(format_date, time.localtime(time.time()))
    
def dateAddInDays(date1, addcount):
    try:
        addtime=datetime.timedelta(days=int(addcount))
        d1Elements=getDateElements(date1)
        d1 = datetime.datetime(d1Elements.tm_year, d1Elements.tm_mon, d1Elements.tm_mday)
        datenew=d1+addtime
        stringYear ='%d' %datenew.year
        stringMonth = '%d' %datenew.month
        stringDate = '%d' %datenew.day
        stringCurrDate = stringYear + "-" + stringMonth + "-" + stringDate
        return stringCurrDate
#        return datenew.strftime(format_date)
    except Exception as e:
        print e
        return None
   
def dateAddInMinutes(timer):
    currTime = datetime.datetime.now()
    preTime = currTime + datetime.timedelta(seconds = 60 * timer)
    stringYear = '%d' %preTime.year
    stringMonth = '%d' %preTime.month
    stringDate = '%d' %preTime.day
    stringHour = '%d' %preTime.hour
    stringMinute = '%d' %preTime.minute
    stringSecond = '%d' %preTime.second
    stringPreTime = stringYear + "-" + stringMonth + "-" + stringDate + " " + stringHour + ":" + stringMinute + ":" + stringSecond
    return stringPreTime

def get_AnalyseLoginInfo(serverID, stringStartTime, stringEndTime, thedays, flag):
#    print serverID
#    print stringStartTime
#    print stringEndTime
    loginDic = {}
    connectServer0DB()
    dbName = GMWeb.confDic[serverID][1][1]

    if '0' == flag:
        if 0 <= thedays and 1 >= thedays:
            logInfoList = db.loginInfoByHour.find({"$and":[{"currDate":{"$gte":stringStartTime}},{"currDate":{"$lte":stringEndTime}},{"serverID":dbName}]})
        elif 1 < thedays:
            logInfoList = db.loginInfoByDay.find({"$and":[{"currDate":{"$gte":stringStartTime}},{"currDate":{"$lte":stringEndTime}},{"serverID":dbName}]})
    elif '1' == flag:
        logInfoList = db.loginInfoByMinute.find({"$and":[{"currDate":{"$gte":stringStartTime}},{"currDate":{"$lte":stringEndTime}},{"serverID":dbName}]})
#    for logInfo in logInfoList:
#        print logInfo
    
    return logInfoList

def get_AnalyseSaveInfo(serverID, stringStartDate, stringEndDate):
    connectServer0DB()
    dbName = GMWeb.confDic[serverID][1][1]
    saveInfoList = db.saveInfo.find({"$and":[{"date":{"$gte":stringStartDate}},{"date":{"$lte":stringEndDate}},{"serverID":dbName}]},{"2":1, "3":1, "4":1, "5":1, "6":1, "7":1, "14":1, "28":1, "56":1, "date":1, "registerCount":1}).sort("date")
#    for saveInfo in saveInfoList:
#        print saveInfo
#    return
    return saveInfoList

def get_AnalyseRechargeUserInfo(serverID, stringStartDate, stringEndDate):
    if 1 == GMWeb.confDic[serverID][4][1]:
        connectDB(serverID)
        rechargeUserInfo = db.event.find({"$and":[{"time":{"$gte":stringStartDate}},{"time":{"$lte":stringEndDate}},{"evid":700},{"param3":26}]}, {"role":1, "param4":1, "time":1})
    return rechargeUserInfo

def get_AnalyseRechargeInfo(serverID):
    connectServer0DB()
    rechargeInfo = db.rechargeInfoByDay.find({"serverID":serverID}, {"date":1, "loginCount":1, "rechargeUserCount":1, "rechargeTimes":1, "currRechargeCount":1, "rechargeRate":1, "ARPU":1, "ARPPU":1, "serverID":1})
    return rechargeInfo

def get_loginCount(stringTime):
    loginHourRole = logInfoDb.event.distinct("role", {"$and":[{"time":{"$gte":stringTime["stringStartTime"]}},{"time":{"$lte":stringTime["stringEndTime"]}},{"evid":1}]})
    loginHourCount = len(loginHourRole)
    return loginHourCount

def get_preRegisterCount(currTime, dbName0):
#    preTime = currTime + datetime.timedelta(seconds = -60 * 60 * 24)
#    stringPreDate = preTime.strftime("%Y-%m-%d")
#    preRegisterInfo = db.loginInfoByDay.find({"currDate":stringPreDate, "serverID":dbName0},{"registerCount":1})
    preRegisterInfo = logInfoDb0.loginInfoByDay.find({"serverID":dbName0},{"registerCount":1}).sort("currDate",pymongo.DESCENDING).limit(1)
    tempCount = preRegisterInfo.count()
    if 0 == tempCount:
        preRegisterCount = 0
    else:
        preRegisterCount = preRegisterInfo[0]["registerCount"]
    logging.info("!!!!pregRegisterCount is %s", preRegisterCount)
    return preRegisterCount

def get_OnlineCount(onlineCount):
    onlineInfo = logInfoDb.serverInfo.find({}, {"playerCount":1})
    onlineCount["currOnlineCount"] = onlineInfo[0]["playerCount"]
    return 

def get_logInfoCount(timer):
    logging.info("GET_LOGINFOCOUNT START!")
    hourFlag = False
    dayFlag = False
    secs = GMWeb.secs
    currTime = datetime.datetime.now()
    preThreadHour = -1
#    stringEndTime = currTime.strftime("%Y-%m-%d %H:%M:%S")
#    if 60 <= currTime.minute + timer:
#        if 23 == currTime.hour:
#            dayFlag = True
#        hourFlag = True
#        preThreadHour = currTime.hour
#        stringEndDate = currTime.strftime("%Y-%m-%d")
#        stringStartTime = stringEndDate + " " + "00:00:00"
#        stringTime = {"stringStartTime":stringStartTime, "stringEndTime":stringEndTime
#    elif 0 <= currTime.minute and 10 > currTime.minute:
#        stringTime = {}
    stringEndTime = currTime.strftime("%Y-%m-%d %H:%M:%S")
    stringEndDate = currTime.strftime("%Y-%m-%d")
    stringStartTime = stringEndDate + " " + "00:00:00"
    stringTime = {"stringStartTime":stringStartTime, "stringEndTime":stringEndTime}
    logging.info("GET_LOGINFOCOUNT___stringTime: %s,___stringEndDate: %s", stringTime, stringEndDate)
    if 60 <= currTime.minute + timer:
        if 23 == currTime.hour:
            dayFlag = True
        hourFlag = True 

    logging.info("GET_LOGINFOCOUNT___dayFlag: %s,___hourFlag: %s", dayFlag, hourFlag)
    logInfo = {'minuteLogCount':0, 'hourLogCount':0, 'dayLogCount':0}
    for i in range(len(secs)):
        logging.info("GET_LOGINFOCOUNT___for begin,secs is: %s", secs)
        try:
            logInfoConnectDB(secs[i])
            onlineExceptFlag = 0
            try:
                onlineInfo = logInfoDb.serverInfo.find({}, {"playerCount":1, "registerCount":1, "rechargeCount":1})
                GMWeb.confDic[secs[i]][4][1] = 1
                state = 1
                onlineExceptFlag = 0
                logging.info("GET_LOGINFOCOUNT___get serverInfo success!")
            except:
                GMWeb.confDic[secs[i]][4][1] = 0
                state = 0
                onlineExceptFlag = 1
                logging.info("GET_LOGINFOCOUNT___get serverInfo except!")
            loginHourCount = get_loginCount(stringTime)
            dbName0 = GMWeb.confDic[secs[i]][1][1]
            logging.info("GET_LOGINFOCOUNT___playerCount is %s___registerCount is %s___loginHourCount is %s___dbName0 is %s", onlineInfo[0]['playerCount'], onlineInfo[0]['registerCount'], loginHourCount, dbName0)
            logInfoConnectServer0DB()
            if onlineExceptFlag:
                logInfoDb0.serverInfo.save({"_id":secs[i], "playerCount":0, "registerCount":0, "rechargeCount":0, "serverState":state})
            if 0 != onlineInfo.count():
                logInfoDb0.serverInfo.save({"_id":secs[i], "playerCount":onlineInfo[0]['playerCount'], "registerCount":onlineInfo[0]['registerCount'], "rechargeCount":onlineInfo[0]['rechargeCount'], "serverState":state})
            else:
                logInfoDb0.serverInfo.save({"_id":secs[i], "playerCount":0, "registerCount":0, "rechargeCount":0, "serverState":state})
            maxOnlineInfo = logInfoDb0.maxPlayerCount.find({"serverID":dbName0},{"maxPlayerCount":1})
            if 0 != maxOnlineInfo.count():
                if onlineInfo[0]["playerCount"] > maxOnlineInfo[0]["maxPlayerCount"]:
                    maxOnlineCount = onlineInfo[0]["playerCount"]
                    logInfoDb0.maxPlayerCount.update({"serverID":dbName0},{"$set":{"maxPlayerCount":maxOnlineCount}})
                else:
                    maxOnlineCount = maxOnlineInfo[0]["maxPlayerCount"]
            else:
                maxOnlineCount = onlineInfo[0]["playerCount"]
                logInfoDb0.maxPlayerCount.insert_one({"serverID":dbName0, "maxPlayerCount":maxOnlineCount})
            logging.info("dbName 0 is %s, maxOnlineCount is %s", dbName0, maxOnlineCount)
            registerCount = onlineInfo[0]["registerCount"]
            preDayRegisterInfoCount =  get_preRegisterCount(currTime, dbName0)
            newRegisterCount = registerCount - preDayRegisterInfoCount
            logging.info("GET_LOGINFOCOUNT___maxOnlineInfoCount is %s___maxOnlineCount is %s___preDayRegisterInfoCount is %s___newRegisterCount is %s", maxOnlineInfo[0]['maxPlayerCount'], maxOnlineCount, preDayRegisterInfoCount, newRegisterCount)
            logInfoDb0.loginInfoByMinute.insert_one({"currDate":stringEndTime, "registerCount":newRegisterCount, "currLoginInfoCount":loginHourCount, "currOnlineCount":onlineInfo[0]["playerCount"], "serverID":dbName0})
            logging.info("GET_LOGINFOCOUNT___loginInfoByMinute.insert success!")
            if hourFlag:
                logInfoDb0.loginInfoByHour.insert_one({"currDate":stringEndTime, "registerCount":newRegisterCount, "currLoginInfoCount":loginHourCount, "currOnlineCount":onlineInfo[0]["playerCount"], "serverID":dbName0})
                logging.info("GET_LOGINFOCOUNT___loginInfoByHour.insert success!")
                if dayFlag:
                    logInfoDb0.maxPlayerCount.update({"serverID":dbName0},{"$set":{"maxPlayerCount":0}})
                    logging.info("GET_LOGINFOCOUNT___maxPlayerCount.update success!")
                    logInfoDb0.loginInfoByDay.insert_one({"currDate":stringEndDate, "registerCount":registerCount, "todayRegisterCount":newRegisterCount, "currLoginInfoCount":loginHourCount, "currOnlineCount":maxOnlineCount, "serverID":dbName0})
                    logging.info("GET_LOGINFOCOUNT___loginInfoByDay.insert success!")
        except:
            logging.info("GET_LOGINFOCOUNT EXCEPT!")
    logging.info("GET_LOGINFOCOUNT END!")
    return

def get_registerInfo(currTime, serverInfo, registerList, loginList):
    logging.info("GET_REGISTERINFO BEGIN!")
    delta = datetime.timedelta(days=-1)
    preTime = currTime + delta
    startTimeX = preTime.strftime('%Y-%m-%d')
    endTimeX = currTime.strftime('%Y-%m-%d')
    dbName = GMWeb.confDic[serverInfo][1][1]
    logging.info("GET_REGISTERINFO___startTimeX is %s___endTimeX is %s___dbName is %s", startTimeX, endTimeX, dbName)
    try:
        saveConnectDB(serverInfo)
        roleCursor = saveDb.event.find({"$and":[{"time":{"$gte":startTimeX}},{"time":{"$lte":endTimeX}},{"evid":6}]},{"role":1})
        logging.info("GET_REGISTERINFO___get roleCurser success!")
        temploginList = saveDb.event.distinct("role", {"$and":[{"time":{"$gte":startTimeX}},{"time":{"$lte":endTimeX}},{"evid":1}]})
        logging.info("GET_REGISTERINFO___get loginList sueecess!")
        loginList.append(temploginList[:])
        for role in roleCursor:
            registerList.append(role['role'])
    except:
        logging.info("GET_REGISTERINFO EXCEPT!")
    logging.info("GET_REGISTERINFO END!")
    return 

def calculateSaveInfo(currTime, registerList, loginList, serverInfo):
    logging.info("CALCULATESAVEINFO BEGIN!")
    delta = datetime.timedelta(days=-1)
    preTime = currTime + delta
    startTimeX = preTime.strftime('%Y-%m-%d')
    dbName = GMWeb.confDic[serverInfo][1][1]
    registerCount = len(registerList)
    try:
        saveConnectServer0DB()
        logging.info("CALCULATESAVEINFO___dbName is %s___startTimeX is %s", dbName, startTimeX)
        saveDb0.saveInfo.insert_one({"date":startTimeX, "registerRole":registerList, "registerCount":registerCount, "2":-1, "3":-1, "4":-1, "5":-1, "6":-1, "7":-1, "14":-1, "28":-1, "56":-1, "serverID":dbName})
        logging.info("CALCULATESAVEINFO___insert success!")
        for i in range(55):
            if 7 < i + 2 and 14 > i + 2:
                continue
            if 14 < i + 2 and 28 > i + 2:
                continue
            if 28 < i + 2 and 56 > i + 2:
                continue
            logging.info("CALCULATESAVEINFO___for begin i is %s", i)
            saveCount = 0
            delta0 = datetime.timedelta(days = -1 * (i + 2))
            preTime = currTime + delta0
            stringCurrDate = preTime.strftime('%Y-%m-%d')
            currRegisterCursor = saveDb0.saveInfo.find({"$and":[{"date":stringCurrDate},{"serverID":dbName}]},{"registerRole":1, "registerCount":1})
            logging.info("CALCULATESAVEINFO___find registerInfo success!")
            if 0 == currRegisterCursor.count():
                logging.info("CALCULATESAVEINFO___currRegiterCount is 0!")
                continue
            else:
                currRegisterList = currRegisterCursor[0]["registerRole"]
                currRegisterCount = currRegisterCursor[0]["registerCount"]
                logging.info("CALCULATESAVEINFO___currRegisterCount is %s", currRegisterCount)
                for roleID in loginList[0]:
                    if 1 <= currRegisterList.count(roleID):
                        print roleID
                        saveCount = saveCount + 1
                logging.info("CALCULATESAVEINFO___saveCount is %s", saveCount)
                if 0 == currRegisterCount:
                    saveRate = 0
                else:
                    tempSaveRate = float(saveCount) / currRegisterCount
                    saveRate = float('%.3f' % tempSaveRate)
                index = i + 2
                if (2 <= index and 7 >= index) or (14 == index) or (28 == index) or (56 == index):
                    stringIndex = str(index)
                    saveDb0.saveInfo.update_one({"$and":[{"serverID":dbName}, {"date":stringCurrDate}]},{"$set":{stringIndex:saveRate}})
                    logging.info("CALCULATESAVEINFO___save update success index is %s", stringIndex)
    except:
        logging.info("CALCULATESAVEINFO EXCEPT!")
    logging.info("CALCULATESAVEINFO END!")
    return
                
def deal_SaveInfo():
    logging.info("DEAL_SAVEINFO BEGIN!")
    secs = GMWeb.secs
    currTime = datetime.datetime.now()
    registerList = []
    loginList = []
    for i in range(len(secs)):
        serverInfo = secs[i]
        logging.info("DEAL_SAVEINFO FOR BEGIN___serverInfo is %s", serverInfo)
        del registerList[:]
        del loginList[:]
        get_registerInfo(currTime, serverInfo, registerList, loginList)       
        calculateSaveInfo(currTime, registerList, loginList, serverInfo)
    logging.info("DEAL_SAVEINFO END!")
    return

def deal_RechargeInfo():
    logging.info("DEAL_RECHARGEINFO BEGIN!")
    secs = GMWeb.secs
    currTime = datetime.datetime.now()
    delta = datetime.timedelta(days=-1)
    delta0 = datetime.timedelta(days = -2)
    preTime = currTime + delta
    preTime0 = currTime + delta0
    startTimeX = preTime.strftime('%Y-%m-%d')
    endTimeX = currTime.strftime("%Y-%m-%d")
    preTimeX = preTime0.strftime('%Y-%m-%d')
    for i in range(len(secs)):
        serverInfo = secs[i]
        try:
            logging.info("DEAL_RECHARGEINFO___for begin serverInfo is %s", serverInfo)
            rechargeConnectDB(serverInfo)
            temploginList = rechargeDb.event.distinct("role", {"$and":[{"time":{"$gte":startTimeX}},{"time":{"$lte":endTimeX}},{"evid":1}]})    
            loginCount = len(temploginList)
            logging.info("DEAL_RECHARGEINFO___get login Count success is %s", loginCount)
            tempRechargeUserList = rechargeDb.event.distinct("role", {"$and":[{"time":{"$gte":startTimeX}},{"time":{"$lte":endTimeX}},{"evid":700, "param3":26}]})
            rechargeUserCount = len(tempRechargeUserList)
            logging.info("DEAL_RECHARGEINFO___get rechargeUserCount success is %s", rechargeUserCount)
            logging.info("DEAL_RECHARGEINFO___db000000000000000 is %s", rechargeDb)
            rechargeTimesCount = rechargeDb.event.find({"$and":[{"time":{"$gte":startTimeX}},{"time":{"$lte":endTimeX}},{"evid":700},{"param3":26}]}, {"role":1}).count()
            logging.info("DEAL_RECHARGEINFO___db111111111111111 is %s", rechargeDb)
            logging.info("DEAL_RECHARGEINFO___get rechargeTimes success is %s", rechargeTimesCount)
            totalRechargeInfos = rechargeDb.serverInfo.find({}, {"rechargeCount":1})
            logging.info("DEAL_RECHARGEINFO___db222222222222222 is %s", rechargeDb)
            totalRechargeCount = totalRechargeInfos[0]["rechargeCount"]
#            logging.info("DEAL_RECHARGEINFO___get totalRechargeCount success is %s", totalRechargeCount)
            logging.info("DEAL_RECHARGEINFO___serverInfo is %s___loginCount is %s___rechargeUserCount is %s___rechargeTimesCount is %s___totalRechargeCount is %s", serverInfo, loginCount, rechargeUserCount, rechargeTimesCount, totalRechargeCount)
            rechargeConnectServer0DB()
            preTotalRechargeInfo = rechargeDb0.rechargeInfoByDay.find({"serverID":serverInfo},{"totalRechargeCount":1, "date":1}).sort("date",pymongo.DESCENDING).limit(1)
            if 0 == preTotalRechargeInfo.count():
                currRechargeCount = totalRechargeCount
            else:
                currRechargeCount = totalRechargeCount - preTotalRechargeInfo[0]['totalRechargeCount'] 
            logging.info("DEAL_RECHARGEINFO___currRechargeCount is %s", currRechargeCount)
            if 0 != loginCount:
                tempRechargeRate = float(rechargeUserCount) / loginCount   
                rechargeRate = float('%.2f' % tempRechargeRate)
                tempARPU = float(currRechargeCount) / loginCount
                ARPU = float('%.2f' % tempARPU)
            else:
                rechargeRate = 0
                ARPU = 0
            if 0 != rechargeUserCount:
                tempARPPU = float(currRechargeCount) / rechargeUserCount
                ARPPU = float('%.2f' % tempARPPU)
            else:
                ARPPU = 0
            rechargeDb0.rechargeInfoByDay.insert_one({"date":startTimeX, "loginCount":loginCount, "rechargeUserCount":rechargeUserCount, "rechargeTimes":rechargeTimesCount, "currRechargeCount":currRechargeCount, "totalRechargeCount":totalRechargeCount, "rechargeRate":rechargeRate, "ARPU":ARPU, "ARPPU":ARPPU, "serverID":serverInfo})
            logging.info("DEAL_RECHARGEINFO insert success!")
        except:
            logging.info("DEAL_RECHARGEINFO EXCEPT!")
    logging.info("DEAL_RECHARGEINFO END!")
    return

def get_Blacklist(serverID):
    connectDB(serverID)
    blacklistCursor = db.blacklist.find({}, {"userid":1, "forbidReason":1, "forcetime":1})
    return blacklistCursor
