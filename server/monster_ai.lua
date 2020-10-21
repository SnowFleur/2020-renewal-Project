
STOP_AND_HELLO=1
FOLLOW=4

UP=0
DOWN=1
LEFT=2
RIGHT=3
NEAR=1
NOT_NEAR=2

my_id=9999;
my_pos_x=0;
my_hp=999;
my_pos_y=0;
my_name="NULL";
my_type=0;
my_level=0;


my_status=STOP_AND_HELLO;

function Set_AI_infor(id,pos_x,pos_y,name,type,level,hp)
my_id=id;
my_pos_x=pos_x;
my_pos_y=pos_y;
my_name=name;
my_type=type;
my_level=level;
my_hp=hp;
end


function event_my_status(Player)
	if(my_status==STOP_AND_HELLO)then
		API_SendMonsterInfo(Player,my_id,my_type,my_level,my_hp,my_name)
	end
end

function event_attak_to_player(Player)
    my_hp=my_hp-10

	if(my_hp<10)then
	API_Requst_Revive(my_level,Player,my_id)
	else
		API_MonsterAttack(Player,my_id)
		API_SendDamageMonster(Player,my_id,my_type,my_level,my_hp,my_name)
	end
end


function event_follwer_to_player(Player)
	player_x=API_GET_X(Player)
	player_y=API_GET_Y(Player)
	Monster_x=API_GET_X(my_id)
	Monster_y=API_GET_Y(my_id)
	is_near=API_GetNearPlayer(Player,my_id)

		if(is_near==NEAR)then
			if(player_x>Monster_x)then
				API_FollowerPlayer(Player,my_id,RIGHT)
			elseif(player_x<Monster_x)then
				API_FollowerPlayer(Player,my_id,LEFT)
			elseif(player_x==Monster_x)then
				if(player_y<Monster_y)then
					API_FollowerPlayer(Player,my_id,UP)
				elseif(player_y>Monster_y)then
					API_FollowerPlayer(Player,my_id,DOWN)
				end
			end
		end
end


