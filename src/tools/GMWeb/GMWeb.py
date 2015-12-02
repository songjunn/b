#-*-coding:utf-8-*-i
import web
import model
import ConfigParser
import threading
import time
import datetime
import json
import string, os, sys
import logging
import urllib
import urllib2
import httplib

logging.basicConfig(level=logging.DEBUG,
                format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                datefmt='%a, %d %b %Y %H:%M:%S',
                filename='log/myapp.log',
                filemode='w')

#################################################################################################
#定义一个StreamHandler，将INFO级别或更高的日志信息打印到标准错误，并将其添加到当前的日志处理对象#
console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)
#################################################################################################

urls = (
	'/', 'Login',
	'/index', 'Index',
        '/playerInfos', 'PlayerInfos',
#        '/userAnalyse/(.+)', 'UserAnalyse',
        '/playerInfo/(.+)/(.+)/(.+)', 'PlayerInfo',
        '/currItem/(.+)/(.+)/(.+)', 'CurrItem',
        '/myHeroes/(.+)/(.+)/(.+)', 'MyHeroes',
        '/heroesItem/(.+)/(.+)/(.+)', 'HeroesItem',
        '/packageInfo/(.+)/(.+)/(.+)', 'PackageInfo',
        '/formationInfo/(.+)/(.+)/(.+)', 'FormationInfo',
        '/userAnalyse/(.+)/logInInfo', 'AlogInInfo',
        '/userAnalyse/(.+)/logInInfo/dataView', 'AloginDataView',
        '/userAnalyse/(.+)/rechargeUserInfo', 'ArechargeUserInfo',
        '/userAnalyse/(.+)/rechargeUserInfo/dataView', 'ArechargeUserInfoDataView',
        '/userAnalyse/(.+)/rechargeInfo', 'ArechargeInfo',
        '/userAnalyse/(.+)/saveInfo', 'AsaveInfo',
        '/userAnalyse/(.+)/saveInfo/dataView', 'AsaveDataView',
        '/blacklist/(.+)', 'Blacklist',
        '/broadcast/(.+)', 'Broadcast',
        '/mail/(.+)', 'Mail',
        '/mailPlayerList/(.+)/(.+)/(.+)', 'MailPlayerList',
        '/mailRetransmission', 'MailRetransmission',
        '/broadcastRetransmission', 'BroadcastRetransmission',
        '/downloadFiles', 'DownloadFiles',
        '/root/GMWeb/downloadFiles/(.+)', 'Download',
        '/logout', 'Logout',
        '/testPost', 'TestPost',
       )

app = web.application(urls, globals())

t_globals = {
	'datestr': web.datestr,
	'cookie': web.cookies,
}

BUF_SIZE = 262144
confDic = {}
secs = []
serverInfoDic = {}
serverInfo0Dic = {}
forbidReasonDic = {}
render = web.template.render('templates', globals = t_globals)
t_globals['render'] = render
t_globals['confDic'] = confDic
t_globals['secs'] = secs
t_globals['serverInfoDic'] = serverInfoDic
login = web.form.Form(
		     web.form.Textbox('username', description = '用户名'),
		     web.form.Password('password', description = '密码'),
		     web.form.Button('login',type = 'submit', html = '登录')
		     )

class TestPost:
    def POST(self):
        print "testPOST"
        print web.input()['name']
        test1 = "2"
        return test1
    
    def GET(self):
        print "testGET"
        print web.input()['name']
        print web.input()['type']
        test1 = "3"
        return test1 

class MailRetransmission:
    def POST(self):
        serverID = web.input()['serverId']
        playerId = web.input()['playerId']
        mailtitleID = web.input()['mailtitle']
        itemID = web.input()['item']
        exp = web.input()['exp']
        silver = web.input()['silver']
        gold = web.input()['gold']
        credit = web.input()['credit']
        ap = web.input()['ap']
        merit = web.input()['merit']
        knight = web.input()['knight']
        serverURL = confDic[serverID][2][1]
        urlList = ["http://", serverURL, ":1818?mType=3&playerid=", playerId, "&mailtitle=", mailtitleID, "&item=", itemID, "&exp=", exp, "&silver=", silver, "&gold=", gold, "&credit=", credit, "&ap=", ap, "&merit=", merit, "&knight=", knight]
        url = ""
        url = "".join(urlList)
#        url = "http://192.168.0.251:8080/testPost?name=dv&type=4"
#        url = "http://192.168.0.162:1818?mType=3&playerid=1"
        print(url)
#        req = urllib2.Request(url)
#        print(req)
#        res_data = urllib2.urlopen(req)
#        res = res_data.read()
#        print res
        ip = serverURL + ":1818"
#        conn = httplib.HTTPConnection(ip)
        flag = 0
        try:
            conn = httplib.HTTPConnection(ip)
            conn.request(method="GET",url=url)
            response = conn.getresponse()
            res = response.read()
            conn.close()
            if "0" == res:
                flag = 1
            elif "1" == res:
                flag = 2
        except:
            print("conn fail")
        return flag

class BroadcastRetransmission:
    def POST(self):
        serverID = web.input()['serverId']
        print(serverID)
        startTime = web.input()['startTime']
        print(startTime)
        endTime = web.input()['endTime']
        print(endTime)
        interval = web.input()['interval']
        print(interval)
        broadcastTimes = web.input()['broadcasttimes']
        print(broadcastTimes)
        content = web.input()['content']
        if "0" == serverID:
            for i in range(len(secs)):
                serverURL = confDic[secs[i]][2][1]
                urlList = ["http://", serverURL, ":1818?mType=2&starttime=", startTime, "&endtime=", endTime, "&interval=", interval, "&broadcasttimes=", broadcastTimes, "&content=", content]
                url = ""
                url = "".join(urlList)
                print(url)
                ip = serverURL + ":1818"
#        conn = httplib.HTTPConnection(ip)
                flag = 1
                try:
                    conn = httplib.HTTPConnection(ip)
                    conn.request(method="GET",url=url)
                    response = conn.getresponse()
                    res = response.read()
                    conn.close()
                    if "0" == res:
                        flag = 1
                    elif "1" == res:
                        flag = 1
                except:
                    print("conn fail")
        else:
            serverURL = confDic[serverID][2][1]
            urlList = ["http://", serverURL, ":1818?mType=2&starttime=", startTime, "&endtime=", endTime, "&interval=", interval, "&broadcasttimes=", broadcastTimes, "&content=", content]
            url = ""
            url = "".join(urlList)
            print(url)
            ip = serverURL + ":1818"
#        conn = httplib.HTTPConnection(ip)
            flag = 0
            try:
                conn = httplib.HTTPConnection(ip)
                conn.request(method="GET",url=url)
                response = conn.getresponse()
                res = response.read()
                conn.close()
                if "0" == res:
                    flag = 1
                elif "1" == res:
                    flag = 2
            except:
                print("conn fail")
        return flag

class Login:
    def GET(self):
        login_form = login()
        posts = model.get_posts()
        return render.login(posts, login_form)

    def POST(self):
        login_form = login()
        if login_form.validates():
           if login_form.d.username == 'admin' and login_form.d.password == 'admin':
               web.setcookie('username', login_form.d.username)
               raise web.seeother('/index')
        raise web.seeother('/')

class Index:
    def GET(self):
        return render.index()

    def POST(self):
        return 'Index POST'

    def __init__(self):
        model.get_ServerInfo(secs)
        logging.info("INDEX___secs is: %s___confDic is: %s", secs, t_globals['confDic'])
        return

class PlayerInfos:
    def GET(self):
#        playerInfos = model.get_PlayerInfos(serverID)
#        playerInfos = selfmodel.get_PlayerInfos()
#        for playerInfo in playerInfos:
#            print playerInfo['name']
        return render.PlayerInfos()
   
    def POST(self):
        formInput = web.input()
        selectType = formInput.get('selectType')
        serverName = formInput.get('serverName')
        selectValue = formInput.get('selectValue')

        if(0 == cmp(selectType, "用户ID")):
            intSelectType = 0
        elif(0 == cmp(selectType, "角色名称")):
            intSelectType = 1
      
        i = 0        
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringType = str(intSelectType)
        stringServerID = str(serverID)
        newURL = "/playerInfo/" + stringType + "/" + selectValue + "/" + stringServerID
        print newURL
        web.seeother(newURL)
        return

class UserAnalyse:
    def GET(self, serverID):
        return render.UserAnalyse(serverID)

    def POST(self):
        return 'UserAnalyse'

class AlogInInfo:
    def GET(self, serverID):
        return render.AlogInInfo(serverID)

    def POST(selt, serverID):
        formInput = web.input()
        serverName = formInput.get('serverName')

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringServerID = str(serverID)
        newURL = "/userAnalyse/" + stringServerID + "/logInInfo"
        print newURL
        web.seeother(newURL)
        return

class AloginDataView:
    totalLogArray = []
    def iniLogArrayEx(self, logArray, dateTimeStart):
        del self.totalLogArray[:]
        timeRange = 73
        for i in range(timeRange):
                tempLogDic = {}
                if(0 == i % 6):
                    theTime = dateTimeStart + datetime.timedelta(hours=i/6)
                    theTimeString = theTime.strftime("%Y-%m-%d %H:%M:%S")
                    tempLogDic['index'] = theTimeString
                else:
                    tempLogDic['index'] = ' '
                tempLogDic['numberIndex'] = i
                tempLogDic['logCount'] = 0
                tempLogDic['registerCount'] = 0
                tempLogDic['onlineCount'] = 0
                self.totalLogArray.append(tempLogDic)
        j = 0
        for loginInfo in logArray:
                stringloginTime = loginInfo['currDate']
                loginTime = datetime.datetime.strptime(stringloginTime,'%Y-%m-%d %H:%M:%S')
                j = ((loginTime - dateTimeStart).seconds)/60/10
                currMinute = loginTime.minute
                currSecond = loginTime.second
                loginTime = (loginTime - datetime.timedelta(minutes = currMinute%10))
                loginTime = (loginTime - datetime.timedelta(seconds = currSecond))
                if j == self.totalLogArray[j]['numberIndex']:
                    if(0 == j % 6):
                        self.totalLogArray[j]['index'] = loginTime
                    else:
                        self.totalLogArray[j]['index'] = ' '
                    self.totalLogArray[j]['logCount'] = loginInfo['currLoginInfoCount']
                    self.totalLogArray[j]['registerCount'] = loginInfo['registerCount']
                    self.totalLogArray[j]['onlineCount'] = loginInfo['currOnlineCount']
                j = j + 1
        return

    def iniLogArray(self, dateTimeStart, thedays, logArray):
        del self.totalLogArray[:]
        if 0 <= thedays and 1 >= thedays:
            timeRange = (thedays + 1) * 24
            for i in range(timeRange):
                index = i % 24
                tempLogDic = {}
                if index < 10:
                   stringIndex = "0" + str(index) + ":00"
                   tempLogDic['index'] = stringIndex
                else:
                   tempLogDic['index'] = str(index) + ":00"
                tempLogDic['numberIndex'] = index
                tempLogDic['logCount'] = 0
                tempLogDic['registerCount'] = 0
                tempLogDic['onlineCount'] = 0
                self.totalLogArray.append(tempLogDic)
            for loginInfo in logArray:
                currTime = loginInfo['currDate']
                divIndex = currTime.index(' ')
                stringCurrDate =  currTime[0:divIndex]
                dateTimeCurr = datetime.datetime.strptime(stringCurrDate,'%Y-%m-%d')
                dateTimeTemp = datetime.datetime.strptime(currTime, '%Y-%m-%d %H:%M:%S')
                betweenDays = (dateTimeCurr - dateTimeStart).days
                tempHour = dateTimeTemp.hour
                currIndex = betweenDays * 24 + tempHour
                self.totalLogArray[currIndex]['logCount'] = loginInfo['currLoginInfoCount']
                self.totalLogArray[currIndex]['registerCount'] = loginInfo['registerCount']
                self.totalLogArray[currIndex]['onlineCount'] = loginInfo['currOnlineCount']
        elif 1 < thedays:
            timeRange = (thedays + 1)
            for i in range(timeRange):
                tempLogDic = {}
                theDate = dateTimeStart + datetime.timedelta(days=i)
                theDateString = theDate.strftime("%Y-%m-%d") 
                tempLogDic['numberIndex'] = i
                tempLogDic['index'] = theDateString
                tempLogDic['logCount'] = 0
                tempLogDic['registerCount'] = 0
                tempLogDic['onlineCount'] = 0
                self.totalLogArray.append(tempLogDic)
            j = 0
            for loginInfo in logArray:
                stringloginDate = loginInfo['currDate']
                loginDate = datetime.datetime.strptime(stringloginDate,'%Y-%m-%d')
                j = (loginDate - dateTimeStart).days
                if j == self.totalLogArray[j]['numberIndex']:
                    self.totalLogArray[j]['logCount'] = loginInfo['currLoginInfoCount']
                    self.totalLogArray[j]['registerCount'] = loginInfo['todayRegisterCount']
                    self.totalLogArray[j]['onlineCount'] = loginInfo['currOnlineCount']
        return 

    def GET(self, serverID):
        print serverID
        flag = web.input()['flag']
        logging.info("ALOGINDATAVIEW")
        if "0" == flag:
            dateRange = web.input()['dateRange']
            divIndex = dateRange.index('—') 
            stringStartDate =  dateRange[0:divIndex]
            stringEndDate = dateRange[divIndex+1:]
            dateTimeStart = datetime.datetime.strptime(stringStartDate,'%Y-%m-%d')
            dateTimeEnd = datetime.datetime.strptime(stringEndDate,'%Y-%m-%d')
            thedays = (dateTimeEnd - dateTimeStart).days
            if 0 <= thedays and 1 >= thedays:
                stringStartTime = stringStartDate
                delta = datetime.timedelta(days=1)
                dateTimeEndX = dateTimeEnd + delta
                stringEndTime = dateTimeEndX.strftime("%Y-%m-%d")
            elif 1 < thedays:
                stringStartTime = stringStartDate
                stringEndTime = stringEndDate   
        elif "1" == flag:
            thedays = 0
            stringStartTime = web.input()['startDate'] + ' ' + web.input()['startTime']
            stringEndTime = web.input()['endDate'] + ' ' + web.input()['endTime']
            dateTimeStart = datetime.datetime.strptime(stringStartTime,'%Y-%m-%d %H:%M:%S')
            currMinute = dateTimeStart.minute
            currSecond = dateTimeStart.second
            dateTimeStart = (dateTimeStart - datetime.timedelta(minutes = currMinute%10))
            dateTimeStart = (dateTimeStart - datetime.timedelta(seconds = currSecond))
#        if 0 <= thedays and 1 >= thedays:
#            stringStartTime = stringStartDate
#            delta = datetime.timedelta(days=1)
#            dateTimeEndX = dateTimeEnd + delta
#            stringEndTime = dateTimeEndX.strftime("%Y-%m-%d")
#        elif 1 < thedays:
#            stringStartTime = stringStartDate
#            stringEndTime = stringEndDate      
        loginCursor = model.get_AnalyseLoginInfo(serverID, stringStartTime, stringEndTime, thedays, flag)
        loginDic = []
        for logInfo in loginCursor:
           del logInfo['_id']
           loginDic.append(logInfo)
        if '0' == flag:
            self.iniLogArray(dateTimeStart, thedays, loginDic)        
        if '1' == flag:
            self.iniLogArrayEx(loginDic, dateTimeStart)           
        return render.AlogDataView(self.totalLogArray)

class ArechargeUserInfo:
    def GET(self, serverID):
#        rechargeUsersInfo = model.get_AnalyseRechargeUserInfo(serverID)
        return render.ArechargeUserInfo(serverID)
 
    def POST(selt, serverID):
        formInput = web.input()
        serverName = formInput.get('serverName')

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringServerID = str(serverID)
        newURL = "/userAnalyse/" + stringServerID + "/rechargeUserInfo"
        print newURL
        web.seeother(newURL)
        return

class ArechargeUserInfoDataView:
    def GET(self, serverID):
        print serverID
        print web.input()['dateRange']
        dateRange = web.input()['dateRange']
        divIndex = dateRange.index('—')

        stringStartDate =  dateRange[0:divIndex]
        stringEndDate = dateRange[divIndex+1:]
        
        print stringStartDate
        print stringEndDate
        rechargeInfoCursor = model.get_AnalyseRechargeUserInfo(serverID, stringStartDate, stringEndDate)

        #return
        return render.ArechargeUserDataView(rechargeInfoCursor)

class ArechargeInfo:
    def GET(self, serverID):
##        payInfoDic = {}
        payInfo = model.get_AnalyseRechargeInfo(serverID)
        return render.ArechargeInfo(serverID, payInfo)
#        if(0 == payDic['loginCount']):
#            payRate = 0
#        else:
#            payRate = payDic['payUserCount']/payDic['loginCount']
#        
#        if(0 == payDic['loginCount']):
#            Arpu = 0
#        else:
#            Arpu = payDic['payCount']/payDic['loginCount']
#        
#        if(0 == payDic['payUserCount']):
#            Arppu = 0
#        else:
#            Arppu = payDic['payCount']/payDic['payUserCount']
#
#        payInfoDic['payRate'] = payRate
#        payInfoDic['Arpu'] = Arpu
#        payInfoDic['Arppu'] = Arppu

    def POST(selt, serverID):
        formInput = web.input()
        serverName = formInput.get('serverName')

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringServerID = str(serverID)
        newURL = "/userAnalyse/" + stringServerID + "/rechargeInfo"
        print newURL
        web.seeother(newURL)
        return


class AsaveInfo:
    def GET(self, serverID):
#        saveInfo = model.get_AnalyseSaveInfo(serverID)
#        return render.AlogDataView(0)
        return render.AsaveInfo(serverID)
        
    def POST(selt, serverID):
        formInput = web.input()
        serverName = formInput.get('serverName')

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringServerID = str(serverID)
        newURL = "/userAnalyse/" + stringServerID + "/saveInfo"
        print newURL
        web.seeother(newURL)
        return

class AsaveDataView:
    def GET(self, serverID):
        print serverID
        print web.input()['dateRange']
        dateRange = web.input()['dateRange']
        divIndex = dateRange.index('—')

        stringStartDate =  dateRange[0:divIndex]
        stringEndDate = dateRange[divIndex+1:]
        print stringStartDate
        print stringEndDate
        saveInfoCursor = model.get_AnalyseSaveInfo(serverID, stringStartDate, stringEndDate)

        return render.AsaveDataView(saveInfoCursor)

class PlayerInfo():
    def GET(self, selectType, playerValue, serverID):
        print selectType
        print serverID
        print playerValue
        playerInfo = model.get_PlayerInfo(selectType, playerValue, serverID)
        print playerInfo
        return render.PlayerInfo(playerInfo, selectType, playerValue, serverID) 

    def POST(self, a, b, c):
        formInput = web.input()
        selectType = formInput.get('selectType')
        serverName = formInput.get('serverName')
        selectValue = formInput.get('selectValue')

        if(0 == cmp(selectType, "用户ID")):
            intSelectType = 0
        elif(0 == cmp(selectType, "角色名称")):
            intSelectType = 1

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringType = str(intSelectType)
        stringServerID = str(serverID)
        newURL = "/playerInfo/" + stringType + "/" + selectValue + "/" + stringServerID
        print newURL
        web.seeother(newURL)
        return

class CurrItem:
    def GET(self, selectType, playerValue, serverID):
        print selectType
        print serverID
        print playerValue
        currItemArray = model.get_CurrItem(selectType, playerValue, serverID)
        return render.currItem(currItemArray, selectType, playerValue, serverID)
        
    def POST(self, a, b, c):
        formInput = web.input()
        selectType = formInput.get('selectType')
        serverName = formInput.get('serverName')
        selectValue = formInput.get('selectValue')

        if(0 == cmp(selectType, "用户ID")):
            intSelectType = 0
        elif(0 == cmp(selectType, "角色名称")):
            intSelectType = 1

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringType = str(intSelectType)
        stringServerID = str(serverID)
        newURL = "/playerInfo/" + stringType + "/" + selectValue + "/" + stringServerID
        print newURL
        web.seeother(newURL)
        return

           
class MyHeroes:
    def GET(self, selectType, playerValue, serverID):
        print selectType
        print serverID
        print playerValue
        myHeroesArrays = model.get_MyHeroes(selectType, playerValue, serverID)
        return render.myHeroes(myHeroesArrays, selectType, playerValue, serverID)

    def POST(self, a, b, c):
        formInput = web.input()
        selectType = formInput.get('selectType')
        serverName = formInput.get('serverName')
        selectValue = formInput.get('selectValue')

        if(0 == cmp(selectType, "用户ID")):
            intSelectType = 0
        elif(0 == cmp(selectType, "角色名称")):
            intSelectType = 1

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringType = str(intSelectType)
        stringServerID = str(serverID)
        newURL = "/playerInfo/" + stringType + "/" + selectValue + "/" + stringServerID
        print newURL
        web.seeother(newURL)
        return


class HeroesItem:
    def GET(self, selectType, playerValue, serverID):
        print selectType
        print serverID
        print playerValue
        return render.heroesItem(selectType, playerValue, serverID)

    def POST(self, a, b, c):
        formInput = web.input()
        selectType = formInput.get('selectType')
        serverName = formInput.get('serverName')
        selectValue = formInput.get('selectValue')

        if(0 == cmp(selectType, "用户ID")):
            intSelectType = 0
        elif(0 == cmp(selectType, "角色名称")):
            intSelectType = 1

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringType = str(intSelectType)
        stringServerID = str(serverID)
        newURL = "/playerInfo/" + stringType + "/" + selectValue + "/" + stringServerID
        print newURL
        web.seeother(newURL)
        return

class PackageInfo:
    def GET(self, selectType, playerValue, serverID):
        print selectType
        print serverID
        print playerValue
        packageInfoArrays = model.get_CurrItem(selectType, playerValue, serverID)
        return render.packageInfo(packageInfoArrays, selectType, playerValue, serverID)

    def POST(self, a, b, c):
        formInput = web.input()
        selectType = formInput.get('selectType')
        serverName = formInput.get('serverName')
        selectValue = formInput.get('selectValue')

        if(0 == cmp(selectType, "用户ID")):
            intSelectType = 0
        elif(0 == cmp(selectType, "角色名称")):
            intSelectType = 1

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringType = str(intSelectType)
        stringServerID = str(serverID)
        newURL = "/playerInfo/" + stringType + "/" + selectValue + "/" + stringServerID
        print newURL
        web.seeother(newURL)
        return


class FormationInfo:
    def GET(self, selectType, playerValue, serverID):
        print selectType
        print serverID
        print playerValue
        playerInfo = model.get_PlayerInfo(selectType, playerValue, serverID)
        print playerInfo
        return render.formationInfo(playerInfo, selectType, playerValue, serverID)

    def POST(self, a, b, c):
        formInput = web.input()
        selectType = formInput.get('selectType')
        serverName = formInput.get('serverName')
        selectValue = formInput.get('selectValue')

        if(0 == cmp(selectType, "用户ID")):
            intSelectType = 0
        elif(0 == cmp(selectType, "角色名称")):
            intSelectType = 1

        i = 0
        for i in range(len(t_globals['secs'])):
            if(0 == cmp(serverName, confDic[t_globals['secs'][i]][0][1])):
                serverID = t_globals['secs'][i]
                break

        stringType = str(intSelectType)
        stringServerID = str(serverID)
        newURL = "/playerInfo/" + stringType + "/" + selectValue + "/" + stringServerID
        print newURL
        web.seeother(newURL)
        return

class Blacklist:
    def GET(self, serverID):
        blacklistCursor = model.get_Blacklist(serverID)
#        for blacklistInfo in blacklistCursor:
#            print blacklistInfo
        return render.blacklist(serverID, forbidReasonDic, blacklistCursor)

class Broadcast:
    def GET(self, serverID):
        return render.broadcast(serverID)

class Mail:
    def GET(self, serverID):
        return render.mail(serverID)

class MailPlayerList:
    def GET(self, serverID, selectType, playerValue):
        print(serverID)
        print(selectType)
        print(playerValue)
        playerListCursor = model.get_PlayerInfo(selectType, playerValue, serverID)
        return render.mailPlayerList(serverID, playerListCursor)

class DownloadFiles:
    def GET(self):
        path = sys.path[0]
        downloadPath = path + "/downloadFiles/"
        fileList = os.listdir(downloadPath)
        print(fileList)
        return render.downloadFiles(downloadPath, fileList)

class Download:
    def GET(self, fileName):
        file_name = fileName
        path = sys.path[0]
        downloadPath = path + "/downloadFiles/"
        file_path = os.path.join(downloadPath, file_name)
        print(file_path)
        f = None
        try:
            f = open(file_path, "rb")
            web.header('Content-Type','application/octet-stream')
            web.header('Content-disposition', 'attachment; filename=%s' % file_name)
            while True:
                c = f.read(BUF_SIZE)
                if c:
                    yield c
                else:
                    break
        except e:
            print(e)
            yield 'Error'
        finally:
            if f:
                f.close()

class Logout:
    def GET(self):
        web.setcookie('username', '', expires = -1)
        raise web.seeother('/')

def notfound1():
    return web.notfound("Sorry, the page you were looking for was not found.")

app.notfound = notfound1

cf = ConfigParser.ConfigParser()
cf.read("GMWeb.conf")
secs = cf.sections()
t_globals['secs'] = cf.sections()

for i in range(len(secs)):
    state = ['state', 0]
    kvs = cf.items(secs[i])
    kvs.append(state)
    confDic[secs[i]] = kvs
    t_globals['confDic'][secs[i]] = kvs


secs.remove('serverInfo0')
t_globals['secs'].remove('serverInfo0')
secs.remove('serverTimer')
t_globals['secs'].remove('serverTimer')
secs.remove('forbidReason')
t_globals['secs'].remove('forbidReason')

serverInfo0Dic['serverInfo0'] = t_globals['confDic']['serverInfo0']
timerDic = t_globals['confDic']['serverTimer']
forbidReasonDic = t_globals['confDic']['forbidReason']
del t_globals['confDic']['serverInfo0']
del t_globals['confDic']['serverTimer']
del t_globals['confDic']['forbidReason']
print t_globals['confDic']
print secs
print timerDic
for i in range(len(forbidReasonDic)):
    print forbidReasonDic[i]
lootMinute = timerDic[0][1]
intLootMinute = int(lootMinute)
intLootMinute = 10

def doChore():
    time.sleep(60 * intLootMinute)

def doSaveChore():
    time.sleep(60 * intLootMinute)

def doRechargeChore():
    time.sleep(60 * intLootMinute)

class BoothThread(threading.Thread):
    def __init__(self, monitor):
        self.monitor = monitor
        threading.Thread.__init__(self)

    def run(self):
        while True:
            monitor['lock'].acquire()
            model.get_logInfoCount(intLootMinute)
            doChore()
            monitor['lock'].release()

class SaveInfoThread(threading.Thread):
    def __init__(self, monitor):
        self.monitor = monitor
        threading.Thread.__init__(self)

    def run(self):
        while True:
            monitor['saveLock'].acquire()
            logging.info("DEAL_SAVEINFO THREAD BEGIN!")
            currTime = datetime.datetime.now()
            hour = currTime.hour
            minute = currTime.minute
            if 0 == hour:
                if 0 <= minute and intLootMinute > minute:
                    model.deal_SaveInfo()            
            doSaveChore()
            monitor['saveLock'].release()
            logging.info("DEAL_SAVEINFO THREAD END!")

class rechargeInfoThread(threading.Thread):
    def __init__(self, monitor):
        self.monitor = monitor
        threading.Thread.__init__(self)

    def run(self):
        while True:
            monitor['rechargeLock'].acquire()
            logging.info("RECHARGEINFO THREAD BEGIN!")
            currTime = datetime.datetime.now()
            hour = currTime.hour
            minute = currTime.minute
            if 0 == hour:
                if 0 <= minute and intLootMinute > minute:
                    model.deal_RechargeInfo()
            doRechargeChore()
            monitor['rechargeLock'].release()
            logging.info("RECHARGEINFO THREEAD END!")

monitor = {'lock':threading.Lock(), 'saveLock':threading.Lock(), 'rechargeLock':threading.Lock()}
new_thread = BoothThread(monitor)
saveThread = SaveInfoThread(monitor)
rechargeThread = rechargeInfoThread(monitor)

if __name__ == '__main__':
    new_thread.start()
    saveThread.start()
    rechargeThread.start()
    app.run() 
