# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: MessageTypeDefine.proto

from google.protobuf.internal import enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)




DESCRIPTOR = _descriptor.FileDescriptor(
  name='MessageTypeDefine.proto',
  package='Message',
  serialized_pb='\n\x17MessageTypeDefine.proto\x12\x07Message*\xf5\x0e\n\tMsgDefine\x12\x14\n\x10MSG_SERVER_BEGIN\x10\x00\x12\x17\n\x13MSG_SERVER_REGISTER\x10\x01\x12\x19\n\x15MSG_SERVER_SYNCSERVER\x10\x02\x12\x1b\n\x17MSG_SERVER_SYNCGATELOAD\x10\x03\x12\x1c\n\x18MSG_SERVER_WORLD_REQUEST\x10\x04\x12\x1d\n\x19MSG_SERVER_WORLD_RESPONSE\x10\x05\x12\x19\n\x15MSG_SERVER_NET_ACCEPT\x10\x06\x12\x18\n\x14MSG_SERVER_NET_CLOSE\x10\x07\x12\x1a\n\x16MSG_SERVER_NET_CONNECT\x10\x08\x12\x1a\n\x16MSG_SERVER_NET_RESPONE\x10\t\x12\x14\n\x10MSG_COMMON_ERROR\x10\x64\x12\x14\n\x10MSG_COMMON_EVENT\x10\x65\x12\x17\n\x12MSG_GAMEOBJ_CREATE\x10\xc8\x01\x12\x17\n\x12MSG_GAMEOBJ_REMOVE\x10\xc9\x01\x12 \n\x1bMSG_GAMEOBJ_OBJFIELD_SETI32\x10\xca\x01\x12 \n\x1bMSG_GAMEOBJ_OBJFIELD_SETI64\x10\xcb\x01\x12 \n\x1bMSG_GAMEOBJ_OBJFIELD_SETSTR\x10\xcc\x01\x12 \n\x1bMSG_GAMEOBJ_OBJFIELD_SETALL\x10\xcd\x01\x12\x1d\n\x18MSG_GAMEOBJ_MAPFIELD_ADD\x10\xce\x01\x12\x1d\n\x18MSG_GAMEOBJ_MAPFIELD_DEL\x10\xcf\x01\x12 \n\x1bMSG_GAMEOBJ_MAPFIELD_SETALL\x10\xd0\x01\x12 \n\x1bMSG_GAMEOBJ_MAPFIELD_SETI32\x10\xd1\x01\x12 \n\x1bMSG_GAMEOBJ_MAPFIELD_SETI64\x10\xd2\x01\x12 \n\x1bMSG_GAMEOBJ_MAPFIELD_SETSTR\x10\xd3\x01\x12\x15\n\x10MSG_GAMEOBJ_SYNC\x10\xd4\x01\x12\x1e\n\x19MSG_GAMEOBJ_SYNC_OBJFIELD\x10\xd5\x01\x12\x1e\n\x19MSG_GAMEOBJ_SYNC_MAPFIELD\x10\xd6\x01\x12\x1c\n\x17MSG_GAMEOBJ_SYNC_FINISH\x10\xd7\x01\x12\x1d\n\x18MSG_GAMEOBJ_LOAD_REQUEST\x10\xd8\x01\x12\x1e\n\x19MSG_GAMEOBJ_LOGIN_REQUEST\x10\xd9\x01\x12\x1b\n\x16MSG_USER_lOGIN_REQUEST\x10\xad\x02\x12\x16\n\x11MSG_USER_DISPLACE\x10\xae\x02\x12\x19\n\x14MSG_USER_GET_GATESVR\x10\xaf\x02\x12\x1c\n\x17MSG_USER_HEART_RESPONSE\x10\xb0\x02\x12\x1d\n\x18MSG_PLAYER_LOGIN_REQUEST\x10\x90\x03\x12\x1e\n\x19MSG_PLAYER_LOGOUT_REQEUST\x10\x91\x03\x12!\n\x1cMSG_PLAYER_CHECKNAME_REQUEST\x10\x92\x03\x12\"\n\x1dMSG_PLAYER_CHECKNAME_RESPONSE\x10\x93\x03\x12\x1a\n\x15MSG_PLAYER_LOAD_COUNT\x10\x94\x03\x12\x19\n\x14MSG_PLAYER_LOAD_DATA\x10\x95\x03\x12\x19\n\x14MSG_PLAYER_LOAD_OVER\x10\x96\x03\x12\x1c\n\x17MSG_PLAYER_SYNC_ATTRINT\x10\x97\x03\x12\x1c\n\x17MSG_PLAYER_SYNC_ATTRI64\x10\x98\x03\x12\x17\n\x12MSG_ITEM_DATA_SYNC\x10\xf4\x03\x12\x19\n\x14MSG_ITEM_DELETE_SYNC\x10\xf5\x03\x12\x1a\n\x15MSG_ITEM_ATTRINT_SYNC\x10\xf6\x03\x12\x1a\n\x15MSG_ITEM_ATTRI64_SYNC\x10\xf7\x03\x12\x14\n\x0eMSG_SERVER_END\x10\xaf\xea\x01\x12\x16\n\x10MSG_CLIENT_BEGIN\x10\xb0\xea\x01\x12\x1c\n\x16MSG_REQUEST_USER_CHECK\x10\xb1\xea\x01\x12\x1d\n\x17MSG_REQUEST_GUEST_CHECK\x10\xb2\xea\x01\x12\x1c\n\x16MSG_REQUEST_USER_LOGIN\x10\xb3\xea\x01\x12\x1d\n\x17MSG_REQUEST_USER_LOGOUT\x10\xb4\xea\x01\x12\x1c\n\x16MSG_REQUEST_USER_HEART\x10\xb5\xea\x01\x12\x1f\n\x19MSG_REQUEST_PLAYER_CREATE\x10\xb6\xea\x01\x12\x1f\n\x19MSG_REQUEST_PLAYER_DELETE\x10\xb7\xea\x01\x12 \n\x1aMSG_REQUEST_PLAYER_OBSERVE\x10\xb8\xea\x01\x12\x1a\n\x14MSG_REQUEST_ITEM_USE\x10\x94\xeb\x01\x12\x1d\n\x17MSG_REQUEST_ITEM_DELETE\x10\x95\xeb\x01\x12\x1b\n\x15MSG_REQUEST_ITEM_SELL\x10\x96\xeb\x01\x12\x1c\n\x16MSG_REQUEST_ITEM_EQUIP\x10\x97\xeb\x01\x12\x1e\n\x18MSG_REQUEST_ITEM_UNEQUIP\x10\x98\xeb\x01\x12\x17\n\x11MSG_REQUEST_DEBUG\x10\xe0\xd4\x03\x12\x1a\n\x14MSG_REQUEST_NET_TEST\x10\xe1\xd4\x03\x12\x14\n\x0eMSG_CLIENT_END\x10\xff\xff\x03')

_MSGDEFINE = _descriptor.EnumDescriptor(
  name='MsgDefine',
  full_name='Message.MsgDefine',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_BEGIN', index=0, number=0,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_REGISTER', index=1, number=1,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_SYNCSERVER', index=2, number=2,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_SYNCGATELOAD', index=3, number=3,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_WORLD_REQUEST', index=4, number=4,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_WORLD_RESPONSE', index=5, number=5,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_NET_ACCEPT', index=6, number=6,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_NET_CLOSE', index=7, number=7,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_NET_CONNECT', index=8, number=8,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_NET_RESPONE', index=9, number=9,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_COMMON_ERROR', index=10, number=100,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_COMMON_EVENT', index=11, number=101,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_CREATE', index=12, number=200,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_REMOVE', index=13, number=201,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_OBJFIELD_SETI32', index=14, number=202,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_OBJFIELD_SETI64', index=15, number=203,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_OBJFIELD_SETSTR', index=16, number=204,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_OBJFIELD_SETALL', index=17, number=205,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_MAPFIELD_ADD', index=18, number=206,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_MAPFIELD_DEL', index=19, number=207,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_MAPFIELD_SETALL', index=20, number=208,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_MAPFIELD_SETI32', index=21, number=209,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_MAPFIELD_SETI64', index=22, number=210,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_MAPFIELD_SETSTR', index=23, number=211,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_SYNC', index=24, number=212,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_SYNC_OBJFIELD', index=25, number=213,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_SYNC_MAPFIELD', index=26, number=214,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_SYNC_FINISH', index=27, number=215,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_LOAD_REQUEST', index=28, number=216,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_GAMEOBJ_LOGIN_REQUEST', index=29, number=217,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_USER_lOGIN_REQUEST', index=30, number=301,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_USER_DISPLACE', index=31, number=302,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_USER_GET_GATESVR', index=32, number=303,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_USER_HEART_RESPONSE', index=33, number=304,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_LOGIN_REQUEST', index=34, number=400,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_LOGOUT_REQEUST', index=35, number=401,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_CHECKNAME_REQUEST', index=36, number=402,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_CHECKNAME_RESPONSE', index=37, number=403,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_LOAD_COUNT', index=38, number=404,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_LOAD_DATA', index=39, number=405,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_LOAD_OVER', index=40, number=406,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_SYNC_ATTRINT', index=41, number=407,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_PLAYER_SYNC_ATTRI64', index=42, number=408,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_ITEM_DATA_SYNC', index=43, number=500,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_ITEM_DELETE_SYNC', index=44, number=501,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_ITEM_ATTRINT_SYNC', index=45, number=502,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_ITEM_ATTRI64_SYNC', index=46, number=503,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_SERVER_END', index=47, number=29999,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_CLIENT_BEGIN', index=48, number=30000,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_USER_CHECK', index=49, number=30001,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_GUEST_CHECK', index=50, number=30002,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_USER_LOGIN', index=51, number=30003,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_USER_LOGOUT', index=52, number=30004,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_USER_HEART', index=53, number=30005,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_PLAYER_CREATE', index=54, number=30006,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_PLAYER_DELETE', index=55, number=30007,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_PLAYER_OBSERVE', index=56, number=30008,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_ITEM_USE', index=57, number=30100,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_ITEM_DELETE', index=58, number=30101,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_ITEM_SELL', index=59, number=30102,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_ITEM_EQUIP', index=60, number=30103,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_ITEM_UNEQUIP', index=61, number=30104,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_DEBUG', index=62, number=60000,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_REQUEST_NET_TEST', index=63, number=60001,
      options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='MSG_CLIENT_END', index=64, number=65535,
      options=None,
      type=None),
  ],
  containing_type=None,
  options=None,
  serialized_start=37,
  serialized_end=1946,
)

MsgDefine = enum_type_wrapper.EnumTypeWrapper(_MSGDEFINE)
MSG_SERVER_BEGIN = 0
MSG_SERVER_REGISTER = 1
MSG_SERVER_SYNCSERVER = 2
MSG_SERVER_SYNCGATELOAD = 3
MSG_SERVER_WORLD_REQUEST = 4
MSG_SERVER_WORLD_RESPONSE = 5
MSG_SERVER_NET_ACCEPT = 6
MSG_SERVER_NET_CLOSE = 7
MSG_SERVER_NET_CONNECT = 8
MSG_SERVER_NET_RESPONE = 9
MSG_COMMON_ERROR = 100
MSG_COMMON_EVENT = 101
MSG_GAMEOBJ_CREATE = 200
MSG_GAMEOBJ_REMOVE = 201
MSG_GAMEOBJ_OBJFIELD_SETI32 = 202
MSG_GAMEOBJ_OBJFIELD_SETI64 = 203
MSG_GAMEOBJ_OBJFIELD_SETSTR = 204
MSG_GAMEOBJ_OBJFIELD_SETALL = 205
MSG_GAMEOBJ_MAPFIELD_ADD = 206
MSG_GAMEOBJ_MAPFIELD_DEL = 207
MSG_GAMEOBJ_MAPFIELD_SETALL = 208
MSG_GAMEOBJ_MAPFIELD_SETI32 = 209
MSG_GAMEOBJ_MAPFIELD_SETI64 = 210
MSG_GAMEOBJ_MAPFIELD_SETSTR = 211
MSG_GAMEOBJ_SYNC = 212
MSG_GAMEOBJ_SYNC_OBJFIELD = 213
MSG_GAMEOBJ_SYNC_MAPFIELD = 214
MSG_GAMEOBJ_SYNC_FINISH = 215
MSG_GAMEOBJ_LOAD_REQUEST = 216
MSG_GAMEOBJ_LOGIN_REQUEST = 217
MSG_USER_lOGIN_REQUEST = 301
MSG_USER_DISPLACE = 302
MSG_USER_GET_GATESVR = 303
MSG_USER_HEART_RESPONSE = 304
MSG_PLAYER_LOGIN_REQUEST = 400
MSG_PLAYER_LOGOUT_REQEUST = 401
MSG_PLAYER_CHECKNAME_REQUEST = 402
MSG_PLAYER_CHECKNAME_RESPONSE = 403
MSG_PLAYER_LOAD_COUNT = 404
MSG_PLAYER_LOAD_DATA = 405
MSG_PLAYER_LOAD_OVER = 406
MSG_PLAYER_SYNC_ATTRINT = 407
MSG_PLAYER_SYNC_ATTRI64 = 408
MSG_ITEM_DATA_SYNC = 500
MSG_ITEM_DELETE_SYNC = 501
MSG_ITEM_ATTRINT_SYNC = 502
MSG_ITEM_ATTRI64_SYNC = 503
MSG_SERVER_END = 29999
MSG_CLIENT_BEGIN = 30000
MSG_REQUEST_USER_CHECK = 30001
MSG_REQUEST_GUEST_CHECK = 30002
MSG_REQUEST_USER_LOGIN = 30003
MSG_REQUEST_USER_LOGOUT = 30004
MSG_REQUEST_USER_HEART = 30005
MSG_REQUEST_PLAYER_CREATE = 30006
MSG_REQUEST_PLAYER_DELETE = 30007
MSG_REQUEST_PLAYER_OBSERVE = 30008
MSG_REQUEST_ITEM_USE = 30100
MSG_REQUEST_ITEM_DELETE = 30101
MSG_REQUEST_ITEM_SELL = 30102
MSG_REQUEST_ITEM_EQUIP = 30103
MSG_REQUEST_ITEM_UNEQUIP = 30104
MSG_REQUEST_DEBUG = 60000
MSG_REQUEST_NET_TEST = 60001
MSG_CLIENT_END = 65535




# @@protoc_insertion_point(module_scope)
