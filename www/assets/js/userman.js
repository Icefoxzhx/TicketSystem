var main_json = [{"total_num" : 0}];
var main_json2 = [{"total_num" : 0}];
$(function(){
	$("#userman_inquire").ajaxForm(function(response_text)
	{
		refresh_main_table();
		if(response_text == "-1")
		{
			main_json = [{"total_num": 0}];
			main_json2 = [{"total_num": 0}];
			$("#card_inquire").slideUp(500);
			$("#card_query_order").slideUp(500);
			$("#userman_query_order_total").slideUp(500);
			userman_show_total();
			userman_write_table(main_json[0].total_num);
		}
		else
		{
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			t = main_json.length;
			var fir = {"total_num" : t};
			main_json.unshift(fir);
			userman_show_total();
			userman_write_table(main_json[0].total_num);
			$("#card_userman_total").slideDown(500);
			$("#card_inquire").slideDown(500);
		}
	 });
});


$(function(){
	$("#modify_profile").ajaxForm(function(response_text)
	{
		$("#card_modify_profile").slideUp(500);
		$("#card_inquire").slideUp(500);
		$("#userman_total").slideUp(500);
		$("#card_query_order").slideUp(500);
		$("#userman_query_order_total").slideUp(500);
		if(response_text == "-1")
			swal("Oops", "修改失败，可能是网络故障，请重试！", "error");
		else
			swal("Success", "信息修改成功！修改后的信息如下：\n“" + response_text +"”", "success");
		var main_json = [{"total_num" : 0}];
	});
});




function refresh_main_table()
{
	for(var i = 1; i <= main_json[0].total_num; i++)
		tab.removeChild(tr[i]);
}
function refresh_query_order_table()
{
	for(var i = 1; i <= main_json2[0].total_num; i++)
		tab.removeChild(tr[i]);
}



function userman_show_total()
{
	$("#userman_total").slideDown(500);
	document.getElementById('userman_total').innerHTML = '总共查询到<b>' + main_json[0].total_num +'</b>名用户';
}
function userman_query_order_total()
{
	$("#userman_query_order_total").slideDown(500);
	document.getElementById('userman_query_order_total').innerHTML = '总共查询到<b>' + main_json2[0].total_num +'</b>笔订单';
}




var tab =document.getElementById("userman_main_table");
var tr = new Array();
tr[1] = 'abc';
function userman_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr[i] = document.createElement("tr");
		num = document.createElement("td");
		num.innerHTML = i;
		usernamee = document.createElement("td");
		usernamee.innerHTML = main_json[i].username;
		namee = document.createElement("td");
		namee.innerHTML = main_json[i].name;
		mailAddrr = document.createElement("td");
		mailAddrr.innerHTML = main_json[i].mailAddr;
		privilege = document.createElement("td");
		privilege.innerHTML = main_json[i].privilege;
		details = document.createElement("td");
		details.innerHTML = '<button class="btn btn-link" style="font-size:14px" onclick=userman_choose(' + i + ');>修改</button>';
		queryy = document.createElement("td");
		queryy.innerHTML = '<button class="btn btn-link" style="font-size:14px" onclick=ticket_query_order_function(' + i + ');>查询信息</button>';
		tab.appendChild(tr[i]);
		tr[i].appendChild(num);
		tr[i].appendChild(usernamee);
		tr[i].appendChild(namee);
		tr[i].appendChild(mailAddrr);
		tr[i].appendChild(privilege);
		tr[i].appendChild(queryy);
		tr[i].appendChild(details);
	}
}




var tab2 =document.getElementById("userman_query_order_table");
var tr2 = new Array();
tr2[1] = 'abc';
function userman_query_ticket_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr2[i] = document.createElement("tr");
		num = document.createElement("td");
		num.innerHTML = i;
		trainid = document.createElement("td");
		trainid.innerHTML = main_json2[i].trainid;
		start_station = document.createElement("td");
		start_station.innerHTML = main_json2[i].start_station;
		start_station_time = document.createElement("td");
		start_station_time.innerHTML = main_json2[i].start_station_time;
		end_station = document.createElement("td");
		end_station.innerHTML = main_json2[i].end_station;
		end_station_time = document.createElement("td");
		end_station_time.innerHTML = main_json2[i].end_station_time;
		price = document.createElement("td");
		price.innerHTML = main_json2[i].price;
		order_number = document.createElement("td");
		order_number.innerHTML = main_json2[i].order_number;
		order_status = document.createElement("td");
		if(main_json2[i].order_status == "[pending]")
			order_status.innerHTML = "<span class='badge badge-warning'>等待候补购票</span>";
		else if(main_json2[i].order_status == "[success]")
			order_status.innerHTML = "<span class='badge badge-primary'>订票完成</span>";
		else if(main_json2[i].order_status == "[refunded]")
			order_status.innerHTML = "<span class='badge badge-danger'>已退票</span>";
		refund = document.createElement("td");
		if(i == 1)
			refund.innerHTML = '<button class="btn btn-link" style="font-size:14px" onclick=ticket_refund(1);>退票</button>';
		else
			refund.innerHTML = '&nbsp&nbsp&nbsp&nbsp退票';
		tab2.appendChild(tr2[i]);
		tr2[i].appendChild(num);
		tr2[i].appendChild(trainid);
		tr2[i].appendChild(start_station);
		tr2[i].appendChild(start_station_time);
		tr2[i].appendChild(end_station);
		tr2[i].appendChild(end_station_time);
		tr2[i].appendChild(price);
		tr2[i].appendChild(order_number);
		tr2[i].appendChild(order_status);
		tr2[i].appendChild(refund);
	}
}


$(function(){
	$("#card_inquire").hide();
	$("#card_modify_profile").hide();
	$("#userman_total").hide();
	$("#card_query_order").hide();
	$("#userman_query_ticket_total").hide();
	$("#userman_query_order_total").hide();
});



function userman_choose(x)
{
	$("#card_modify_profile").slideDown(500);
	$("#card_query_order").slideUp(500);
	$("#userman_query_order_total").slideUp(500);
	document.getElementById('modify_username').value = main_json[x].username;
	document.getElementById('modify_name').value = main_json[x].name;
	document.getElementById('modify_mailAddr').value = main_json[x].mailAddr;
	document.getElementById('modify_privilege').value = main_json[x].privilege;
}


function ticket_query_order_function(x)
{
	$("#card_modify_profile").slideUp(500);
	$.ajax({
        type: "POST",
        dataType: "json",
        url: "ticket_query_order.php", 
        data: {username: main_json[x].username},
        success: function (response_text2) {
            swal(JSON.stringify(response_text2));
            refresh_query_order_table();
			if(response_text2 == "no")
			{
				main_json2 = [{"total_num": 0}];
				$("#card_query_order").hide();
			}
			else
			{
				$("#card_userman_query_order_total").show();
				$("#card_query_order").show();
				main_json2 = response_text2;
			}
			userman_query_order_total();
			userman_query_ticket_write_table(main_json2[0].total_num);
        },
    });
}

function ticket_refund(x)
{
	$.ajax({
        type: "POST",
        dataType: "text",
        url: "ticketrefund.php", 
        data: {username: main_json[x].username},
        success: function (response_text3) {
			if(response_text3 == "no")
				swal("Oops", "退票失败，出现了意想不到的问题……", "error");
			else
			{
				swal(response_text3);
				$("#card_query_order").hide();
				$("#userman_query_ticket_total").hide();
				swal("Success", "退票成功！", "success");
			}
        },
    });
}