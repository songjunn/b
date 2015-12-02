NetUnit = {}

RegMessage(500, "NetUnit", "OnNetTest")

NetUnit.OnNetTest = function(socket, no, sdata)
	NetTest(socket, no, sdata)
end
	