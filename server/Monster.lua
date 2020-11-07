
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

function State()

end



