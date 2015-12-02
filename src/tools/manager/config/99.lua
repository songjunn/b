----服务器启动时需要读取的配置信息

------------------------------服务器部署配置---------------------------------
Sconf = {}

--//区服编号
Sconf.WorldID                  = 99 

--//服务进程ip设置
Sconf.CentralServer_ip         = "203.195.184.32"
Sconf.DataServer_ip            = "203.195.184.32"
Sconf.GateServer_ip            = "203.195.184.32"
Sconf.GateServer_inetip        = "203.195.184.32"
Sconf.LoginServer_ip           = "203.195.184.32"
Sconf.LoginServer_inetip       = "203.195.184.32"
Sconf.BIServer_ip              = "203.195.184.32"
Sconf.GameServer_ip            = "203.195.184.32"
Sconf.PaymentServer_ip         = "203.195.184.32"
Sconf.PaymentServer_inetip     = "203.195.184.32"

--//服务进程id设置
Sconf.CentralServer_id         = Sconf.WorldID * 1000 + 1
Sconf.DataServer_id            = Sconf.WorldID * 1000 + 2
Sconf.WorldServer_id           = Sconf.WorldID * 1000 + 3
Sconf.LoginServer_id           = Sconf.WorldID * 1000 + 4
Sconf.BIServer_id              = Sconf.WorldID * 1000 + 5
Sconf.GameServer_id            = Sconf.WorldID * 1000 + 100
Sconf.GateServer_id            = Sconf.WorldID * 1000 + 200
Sconf.PaymentServer_id         = Sconf.WorldID * 1000 + 11

--//服务进程端口设置
Sconf.CentralServer_port       = 20010
Sconf.DataServer_port          = 20020
Sconf.WorldServer_port         = 20030
Sconf.LoginServer_port         = 20040
Sconf.BIServer_port            = 20050
Sconf.GameServer_port          = 20100
Sconf.GateServer_port          = 20200
Sconf.PaymentServer_port       = 20001
Sconf.LoginServer_Client_port  = 20800
Sconf.GateServer_Client_port   = 20900
Sconf.PaymentServer_Client_port= 8080

--//版本号
Sconf.Version                  = "1.0.0"

--//数据库参数
Sconf.db_game_ip               = "203.195.184.32"
Sconf.db_game_port             = "27017"
Sconf.db_game_name             = "odin_gamedb_099"
Sconf.db_event_ip              = "203.195.184.32"
Sconf.db_event_port            = "27017"
Sconf.db_event_name            = "odin_gamedb_099"


------------------------------Key--------------------------------------
Key = {}

--//游戏世界参数
Key.PlayerMax                  = 100                      	--服务器总人数
Key.PlayerLoadMax              = 3000                       --DB预加载人数

--//网络消息参数
Key.server_net_connect         = 10                         --selectio最大连接数
Key.packet_pool_size           = 10240                      --消息包缓冲区
Key.recv_buff_size             = 20480                      --selectio双缓冲接收区
Key.send_buff_size             = 20480                      --selectio双缓冲发送区
Key.client_net_buff            = 10240                      --对外网络双缓冲区

--//服务器首次对外开放时间,新服务器对外开放时配置
Key.ServerOpenTime	       = "2015-1-1-0:0:0"

--//监视输出路径
Key.MonitorPath               = "./monitor"


------------------------------CentralServer---------------------------------
CentralServer = {}
CentralServer.connect_count_max              = Key.server_net_connect     --最大连接数
CentralServer.packet_pool_size               = Key.packet_pool_size       --packet pool缓冲区大小
CentralServer.recv_buff_size                 = Key.recv_buff_size         --接收缓冲区大小
CentralServer.send_buff_size                 = Key.send_buff_size         --发送缓冲区大小


------------------------------DataServer---------------------------------
DataServer = {}
DataServer.connect_count_max             	   = Key.server_net_connect     --最大连接数
DataServer.packet_pool_size                  = Key.packet_pool_size       --packet pool缓冲区大小
DataServer.recv_buff_size                    = Key.recv_buff_size         --接收缓冲区大小
DataServer.send_buff_size                    = Key.send_buff_size         --发送缓冲区大小
DataServer.user_count_max                    = Key.PlayerMax              --user数量
DataServer.player_count_max                  = Key.PlayerMax              --player数量
DataServer.item_count_max                  	 = Key.PlayerMax*100          --道具数量
DataServer.hero_count_max                    = Key.PlayerMax*50           --hero
DataServer.save_logic_interval               = 10                         --存盘时间间隔ms
DataServer.db_operator_max                   = 10000


------------------------------GateServer---------------------------------
GateServer = {}
GateServer.connect_count_max             	   = Key.server_net_connect     --最大连接数
GateServer.packet_pool_size                  = Key.packet_pool_size       --packet pool缓冲区大小
GateServer.recv_buff_size                    = Key.recv_buff_size         --接收缓冲区大小
GateServer.send_buff_size                    = Key.send_buff_size         --发送缓冲区大小
GateServer.user_count_max                    = Key.PlayerMax              --user数量
GateServer.player_count_max                  = Key.PlayerMax              --player数量
GateServer.gateway_connect_max               = GateServer.user_count_max
GateServer.gateway_queue_max                 = 40960
GateServer.gateway_recv_size                 = Key.client_net_buff
GateServer.gateway_send_size                 = Key.client_net_buff * 10
GateServer.user_heart_timeout                = 60				  		--心跳超时时间
GateServer.user_key_timeout					         = 30						  --连接密钥超时时间
GateServer.user_pack_limit					         = 30						  --平均每秒发包数量


------------------------------GameServer---------------------------------
GameServer = {}
GameServer.connect_count_max             	   = Key.server_net_connect     --最大连接数
GameServer.packet_pool_size                  = Key.packet_pool_size       --packet pool缓冲区大小
GameServer.recv_buff_size                    = Key.recv_buff_size         --接收缓冲区大小
GameServer.send_buff_size                    = Key.send_buff_size         --发送缓冲区大小
GameServer.user_count_max                    = Key.PlayerMax              --user数量
GameServer.player_count_max                  = Key.PlayerMax              --player数量
GameServer.item_count_max                  	 = Key.PlayerMax*100          --道具数量
GameServer.hero_count_max                    = Key.PlayerMax*50           --hero


------------------------------LoginServer---------------------------------
LoginServer = {}
LoginServer.connect_count_max             	 = Key.server_net_connect     --最大连接数
LoginServer.packet_pool_size                 = Key.packet_pool_size       --packet pool缓冲区大小
LoginServer.recv_buff_size                   = Key.recv_buff_size         --接收缓冲区大小
LoginServer.send_buff_size                   = Key.send_buff_size         --发送缓冲区大小
LoginServer.user_count_max                   = Key.PlayerMax              --user数量
LoginServer.gateway_connect_max              = LoginServer.user_count_max
LoginServer.gateway_queue_max                = 40960
LoginServer.gateway_recv_size                = Key.client_net_buff
LoginServer.gateway_send_size                = Key.client_net_buff
LoginServer.user_heart_timeout				       = 15                         --心跳超时时间
LoginServer.db_operator_max                  = 1000


------------------------------BIServer---------------------------------
BIServer = {}
BIServer.connect_count_max                   = Key.server_net_connect     --最大连接数
BIServer.packet_pool_size                    = Key.packet_pool_size       --packet pool缓冲区大小
BIServer.recv_buff_size                      = Key.recv_buff_size         --接收缓冲区大小
BIServer.send_buff_size                      = Key.send_buff_size         --发送缓冲区大小
BIServer.db_operator_max                     = 1000

------------------------------PaymentServer---------------------------------
PaymentServer = {}
PaymentServer.connect_count_max                   = Key.server_net_connect     --最大连接数
PaymentServer.packet_pool_size                    = Key.packet_pool_size       --packet pool缓冲区大小
PaymentServer.recv_buff_size                      = Key.recv_buff_size         --接收缓冲区大小
PaymentServer.send_buff_size                      = Key.send_buff_size         --发送缓冲区大小
