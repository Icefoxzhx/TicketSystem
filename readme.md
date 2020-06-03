项目要求

https://github.com/oscardhc/TicketSystem-2020

参考资料

https://github.com/spectrometerHBH/TTRs

https://github.com/ZYHowell/Ticket_Booking_System

https://github.com/AryaGuo/Train-ticket-booking-system

https://github.com/jinhongyii/TTRS-final

17作业要求：https://github.com/camelop/ticket_office_cw-DS2018

git教程：https://www.liaoxuefeng.com/wiki/896043488029600

## Questions

- add_user的cur_username是登陆了的吗

要判断是否已经登陆



- buy_ticket返回一定在int内吗qwq

- query_order如果未登录返回什么



- if_delete是否假装删除？
- order 为火车在站里的位置？？？？
- 用位运算存储第i辆火车是否经过某一站点   那么此时的station中的起止时间是否？？？



- query_train中的sum_price是否在int范围内



- hzstring里面是否要再把char*的重载运算符写一遍？




## Notes

- 注意在输入输出的时候某些东西的UTF-8转码

- 总票价在int范围之内

- add_train / add_user的时候要先判断id是否冲突



- string 改成 char 数组 则固定长度
- 多日期存成好多好多量
- 假装删除
- order + 时间戳



- add_train中的票价是两站之间的票价，query_train中的票价是累计票价



- 注意有些int在读入的时候是string
- string都用char!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



- 在hzstring中的转换？

off_t

## bptree

bptree: (key, value, size, comp)




说明：bp_tree里面的基本信息默认只有在modify_profile的时候会变化，而列车/车站的基本信息默认不变（看到会变的话和我说一下啊）

### bptree_user

key:(username)

存储

1.基本信息：username, password, name, mail, privilege, login(判断是否登录了)



修改

1.add_user需要创建新节点

2.login/logout (这个在主程序里面标记一下就好了吧？)

3.modify_profile 会修改基本信息

4.buy_ticket step2添加该用户的购票信息

查询

1.query_profile 返回的是所有的基本信息

2.query_order 返回所有购票信息

3.refund_order 返回指定的购票信息


### bptree_station

key:(station_id, train_id)

存储

1.基本信息：station_id, train_id, arrive_time, depart_time, price(为当前train_id从这一站到下一站的票价), sale_date

2.维护信息：left_ticket(当前train_id从这一站到下一站还剩的票数), 候补队列（当前train_id从这一站到下一站的候补队列，这要用什么存呢？）

修改

1.add_train 添加从station_1...station_num的新节点

2.release_train 将所有涉及到的相关节点标为发布

3.delete_train 将所涉及到的相关节点删除

4.buy_ticket 

step1 查询现在能买几张

step2 修改username的购票信息 见bp_username

step3 修改现在购买多少张票，若需要候补则挂候补票


查询

1.query_ticket 查询station_start和station_end的列车，取交集(且均在限定日期中)

2.query_transfer 查询station_start和station_end的列车，for_1枚举station_start中符合限定日期的列车，for_2枚举中转站，查询该中转站的列车，与station_end作比较



### bptree_train

key:(train_id)

存储

1.基本信息：station_number, station_id, seat_number, start_time, travel_time, stopover_time, sale_date, type, release(标记是否发布)

修改

1.add_train 添加train_id，标为未发布

2.release_train 标为发布

3.delete_train 删除该节点

查询

1.query_train 找到(train_id)，判断sale_date是否在查询范围内




### bptree_order

key:(user_id, buy_date/buy_number(////////////), train_id, start_station, end_station)

value:
1. user_id, buy_date, train_id, start_station, end_station, ticket_num, travel_date, queue(是否候补), status(状态：买票/候补/退票)


QQQQQQQQQQQQq

为什么他们的ticket_key的operator<是有些正序有些倒序？？？？



### bptree_refund

key: (user_id, buy_time)

value:()




## 存储说明

### class_ticket

### class_station

### class_user

### class_train















### bpt_train

key : train_id

value: Train ( )



### bpt_station

key : station_id , train_id

value : int ( the order number of the station in the train)



### bpt_record

key : 



### bpt_refund

key : 

















### route_file

里面存储每个train_id对应的所有station的信息

(station_id, arrive_time, depart_time, price, ticket_left)

price 是和上一站的票价

ticket_left是到下一站的剩余票数



### tl_file

每天(0601(0)-0905(96))的剩余票数，如果这天不售票则-1

每站的date按照arrive_time计算

每天*每站