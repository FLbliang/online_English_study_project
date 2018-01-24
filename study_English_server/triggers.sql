#创建插入学习层次的触发器，以自动创建对应的题目类型记录
delimiter $$
drop trigger if exists add_level_trigger;
create trigger add_level_trigger
	after insert on level_info for each row
begin
	insert into type_info(level_id, type_name) values 
		(new.level_id, '单词学习'),
		(new.level_id, '短语学习'),
		(new.level_id, '句子翻译'),
		(new.level_id, '完型填空'),
		(new.level_id, '阅读理解'),
		(new.level_id, '写作');
end
$$
delimiter ;

#批量插入学习层次记录

insert into level_info(level_name) values('英语四级水平'), ('英语六级水平'), ('考研水平');

#创建删除学习层次的触发器，以自动删除相应的联系记录
delimiter $$
drop trigger if exists del_level_trigger;
create trigger del_level_trigger
	before delete on level_info for each row
begin
	delete from type_info where level_id = old.level_id;
	delete from test_info where level_id = old.level_id;
	delete from user_level_info where level_id = old.level_id;
end
$$
delimiter ;

#创建删除学习类型的触发器，以自动删除相应的联系记录
delimiter $$
drop trigger if exists del_type_trigger;
create trigger del_type_trigger
	before delete on type_info for each row
begin
	delete from problem_info where type_id = old.type_id;
	delete from user_type_info where type_id = old.type_id;
end
$$
delimiter ;


#创建插入用户信息时, 自动创建对应的各个用户荣誉信息记录，选择层次记录，选择类型记录，题目用户联系记录
delimiter $$
drop trigger if exists add_user_trigger;
create trigger add_user_trigger
	after insert on user_info for each row
begin
	declare tmp_level_id int;
	declare tmp_type_id int;
	declare tmp_problem_id int;
	declare flag int default 0;

	declare result_type cursor for select type_id from type_info;
	declare result_level cursor for select level_id from level_info;
	declare result_problem cursor for select problem_id from problem_info;
	declare continue handler for not found set flag = 1;

	open result_type;
	repeat
		fetch result_type into tmp_type_id;
		if flag = 0 then
			insert into user_type_info(user_id, type_id)
				values (new.user_id, tmp_type_id);
		end if;
	until flag = 1 end repeat;
	close result_type;

	set flag = 0;
	open result_level;
	repeat
		fetch result_level into tmp_level_id;
		if flag = 0 then
			insert into user_level_info(user_id, level_id)
				values (new.user_id, tmp_level_id);
		end if;
	until flag = 1 end repeat;
	close result_level;

	set flag = 0;
	open result_problem;
	repeat
		fetch result_problem into tmp_problem_id;
		if flag = 0 then
			insert into user_problem_info(user_id, problem_id)
				values (new.user_id, tmp_problem_id);
		end if;
	until flag = 1 end repeat;
	close result_problem;
end
$$
delimiter ;

#删除用户信息时的触发器
delimiter $$
drop trigger if exists del_user_trigger;
create trigger del_user_trigger
	before delete on user_info for each row
begin
	delete from user_level_info where user_id = old.user_id;
	delete from user_type_info where user_id = old.user_id;
	delete from user_problem_info where user_id = old.user_id;
	delete from security_info where user_id = old.user_id;
	delete from user_message_info where send_user_id = old.user_id or recv_user_id = old.user_id;
	delete from friend_info where user1_id = old.user_id or user2_id = old.user_id;
	delete from apply_info where send_user_id = old.user_id or recv_user_id = old.user_id;
	delete from article_info where host_user_id = old.user_id;
	delete from test_info where user_id = old.user_id;
	delete from reply_info where reply_user_id = old.user_id;
	delete from reply_message_info where send_user_id = old.user_id;
end
$$
delimiter ;

#删除帖子时的触发器，同时删除回帖
delimiter $$
drop trigger if exists del_article_trigger;
create trigger del_article_trigger
	before delete on article_info for each row
begin
	delete from reply_info where article_id = old.article_id;
end
$$

delimiter ;

#删除回帖时的触发器，同时删除回帖留言
delimiter $$
drop trigger if exists del_reply_info_trigger;
create trigger del_reply_info_trigger
	before delete on reply_info for each row
begin
	delete from reply_message_info where reply_id = old.reply_id;
end
$$

delimiter ;

#删除套卷时的触发器，同时删除出题题目
delimiter $$
drop trigger if exists del_test_info_trigger;
create trigger del_test_info_trigger
	before delete on test_info for each row
begin
	delete from test_problem_info where test_id = old.test_id;
end
$$

delimiter ;

#删除具体题目时的触发器，同时删除相应的联系记录
delimiter $$
drop trigger if exists del_problem_info_trigger;
create trigger del_problem_info_trigger
	before delete on problem_info for each row
begin
	delete from select_info where problem_id = old.problem_id;
	delete from test_problem_info where problem_id = old.problem_id;
	delete from user_problem_info where problem_id = old.problem_id;
end
$$

delimiter ;

