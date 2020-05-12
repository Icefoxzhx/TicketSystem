项目要求

https://github.com/oscardhc/TicketSystem-2020

参考资料

https://github.com/spectrometerHBH/TTRs

https://github.com/ZYHowell/Ticket_Booking_System

https://github.com/AryaGuo/Train-ticket-booking-system

git教程：https://www.liaoxuefeng.com/wiki/896043488029600

## Questions

1.候补？（会影响到username的购票信息）

会调用到候补的操作：buy_ticket，refund_ticket

朴素算法：对于每个站点的每个train_ID都记录候补情况（username的第几个购票信息），那么buy_ticket的时候只需要打标记，总时间复杂度O(10^8)；在refund_ticket的时候将所有经过该票区间的候补信息全部遍历一遍（有一个问题，应该取出所有涉及这一段区间的购票信息，按照操作时间从早到晚判断）这个的时间复杂度不会算了...


2.query_transfer(有没有更好的算法啊qwq我把朴素的写在bptree_station-查询-3里面了)

什么样子的是最优解啊qaq

## bptree

说明：bp_tree里面的基本信息默认只有在modify_profile的时候会变化，而列车/车站的基本信息默认不变（看到会变的话和我说一下啊）

### bptree_user

key:(username)

存储

1.基本信息：username, password, name, mail, privilege

2.购票信息：用数组（vector？）按照购票时间顺序存储，可能会修改购票的状态

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

1.基本信息：start_stop_time, end_stop_time, price(为当前train_id从这一站到下一站的票价), sale_date, release(标记是否发布该车辆)

2.维护信息：left_ticket(当前train_id从这一站到下一站还剩的票数), 候补队列（当前train_id从这一站到下一站的候补队列，这要用什么存呢？）

修改

1.add_train 添加从station_1...station_num的新节点，并标为未发布

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


## 存储说明

### class_ticket

### class_station

### class_user

### class_train