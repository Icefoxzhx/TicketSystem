项目要求
https://github.com/oscardhc/TicketSystem-2020

参考资料
https://github.com/spectrometerHBH/TTRs
https://github.com/ZYHowell/Ticket_Booking_System
https://github.com/AryaGuo/Train-ticket-booking-system
git教程：https://www.liaoxuefeng.com/wiki/896043488029600

## Questions
1. 候补？（会影响到 user的购票信息）
会调用到候补的操作：buy_ticket，refund_ticket
朴素算法：对于每个站点的每个train_ID都记录候补情况（username的第几个购票信息），那么buy_ticket的时候只需要打标记，总时间复杂度O(10^8)；在refund_ticket的时候将所有经过该票区间的候补信息全部遍历一遍（有一个问题，应该取出所有涉及这一段区间的购票信息，按照操作时间从早到晚判断）这个的时间复杂度不会算了...



## bp_tree

说明：bp_tree里面的基本信息默认只有在modify_profile的时候会变化，而列车/车站的基本信息默认不变（看到会变的话和我说一下啊）

### bptree_user
key:<username>
存储
1.基本信息：username, password, name, mail, privilege
2.购票信息：用数组（？）按照购票时间顺序存储，可能会修改购票的状态

### bptree_station
key:<station_id, train_id>
存储
1.基本信息：start_stop_time, end_stop_time, price(???), sale_date
2.维护信息：left_ticket, 候补（？）

### bptree_train
key:<train_id>
存储
1.基本信息：station_number, station_id, seat_number, start_time, travel_time, stopover_time, sale_date, type



