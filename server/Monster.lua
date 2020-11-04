
IDLE	=1
ATTACK	=2
MOVE	=3

MyStatue=IDLE

-- ID, X,Y,이름,타입,레벨,공격력,경험치,체력
Myid,MyPosX,MyPosY,MyName,MyType,MyLevel,MyDamage,MyExep,MyHp


function  SetMonsterInfor(id,pos_x,pos_y,name,type,level,hp,damage)
Myid=id
MyPosX=pos_x
MyPosY=pos_y
MyName=name
MyType=type
MyLevel=level
MyDamage=damage
MyHp=hp
end

function  State()

	if(myStatue==IDLE)then
		else if(myStatue==ATTACK)then


		end
		else if(myStatue==MOVE)then
	
		end

	end
end

