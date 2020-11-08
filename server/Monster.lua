
IDLE	=1
ATTACK	=2
MOVE	=3

MyStatue=IDLE

-- ID, X,Y,이름,타입,레벨,공격력,경험치,체력
Myid=0
MyPosX=0
MyPosY=0
MyName=0
MyType=0
MyLevel="NULL"
MyDamage=0
MyExep=0
MyH=0


function SetMonsterInfor(id,pos_x,pos_y,name,type,level,hp,damage,exp)
	Myid=id
	MyPosX=pos_x
	MyPosY=pos_y
	MyName=name
	MyType=type
	MyLevel=level
	MyHp=hp
	MyDamage=damage
	MyExep=exp
end

function GetMonsterInfor()
	API_GetMonsterInfor(Myid,MyPosX,MyPosY,MyName,MyType,MyLevel,MyDamage,MyHp,MyExep)
end

function ChangeMySate(state)
	MyStatue=state
end

function State()

	if MyStatue==IDLE then
	--[[       
	IDEL 상태에서는 스택에 저장했던 내용을 기반으로 다시 돌아간다?

	]]--
	elseif MyStatue==MOVE then
	--[[   
	플레이어가 자신과 직선에 있다면 그냥 직직하고
	그렇지 않다면 A* 실행
	]]--

	else if MyStatue==ATTACK then

	end

end



