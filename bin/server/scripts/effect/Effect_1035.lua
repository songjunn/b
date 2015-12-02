--普通攻击时有x%概率变成群攻(全攻/十字攻/行攻/列攻)
Effect_1035 = {}
Effect_1035.OnEffect = function(role, attr, value)

	ChangePercent(role, attr, value);
	
end

Effect_1035.UnEffect = function(role, attr, value)

	ChangePercent(role, attr, -value);
	
end
