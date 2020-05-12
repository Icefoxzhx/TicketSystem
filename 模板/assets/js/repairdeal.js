var main_json = [{"total_num" : 0}];
$(function(){
	$("#repairdeal_inquire").ajaxForm(function(response_text)
	{
	//	alert(response_text);
		refresh_main_table();
		if(response_text == "PC404")
			main_json = [{"total_num": 0}];
		else
		{
	//		alert(response_text);
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			t = main_json.length;
			var fir = {"total_num" : t};
			main_json.unshift(fir);
		}
		repairdeal_show_total();
		repairdeal_write_table(main_json[0].total_num);
	 });
});
function refresh_main_table()
{
	for(var i = 1; i <= main_json[0].total_num; i++)
		tab.removeChild(tr[i]);
}

function repairdeal_show_total()
{
//	alert("dzsdfaef");
	document.getElementById("repairdeal_total").innerHTML = "总共查询到<b>" +main_json[0].total_num + "</b>条记录";
}

var tab =document.getElementById("repairdeal_main_table");
var tr = new Array();
tr[1] = 'abc';
function repairdeal_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr[i] = document.createElement("tr");
		num = document.createElement("td");
		num.innerHTML = i;
		tg_id = document.createElement("td");
		tg_id.innerHTML = main_json[i].tg_id;
		owner = document.createElement("td");
		owner.innerHTML = main_json[i].owner_id;
		user = document.createElement("td");
		user.innerHTML = main_json[i].user_id;
		buy_date = document.createElement("td");
		buy_date.innerHTML = main_json[i].buy_date;
		lent_date = document.createElement("td");
		lent_date.innerHTML = main_json[i].lent_date;
		pc_brand = document.createElement("td");
		pc_brand.innerHTML = main_json[i].pc_brand;
		pc_model = document.createElement("td");
		pc_model.innerHTML = main_json[i].pc_model;
		repair_id = document.createElement("td");
		repair_id.innerHTML = main_json[i].repair_id;
		describe = document.createElement("td");
		describe.innerHTML = main_json[i].describe;
		repair_status = document.createElement("td");
		if (main_json[i].repair_status == 0)
			repair_status.innerHTML = "待处理";
		else if (main_json[i].repair_status == 1)
			repair_status.innerHTML = "已处理";
		details = document.createElement("td");
		//details_button = document.createElement("button");
		// details_button.setAttribute("class", "btn btn-link repairdeal_show_details_button");
		// details_button.setAttribute("style", "font-size:12px");
		// details_button.setAttribute("onxlick", "font-size:12px");
		// details_button.setAttribute("id", i);
		// text_details_button = document.createTextNode("详情");
		details.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=repairdeal_show_details(' + i + ');>详情</button>';
		change_stat = document.createElement("td");
		if (main_json[i].repair_status == 1)
			change_stat.innerHTML = '<p style = "font-size:12px; font-color:green">已处理</p>'
		else 
			change_stat.innerHTML = '<button class="btn btn-link" style="font-size:12px;color:red" onclick=repairdeal_now_deal(' + main_json[i].repair_id + ');>点此处理</button>';
		tab.appendChild(tr[i]);
		tr[i].appendChild(num);
		tr[i].appendChild(tg_id);
		tr[i].appendChild(owner);
		tr[i].appendChild(user);
		tr[i].appendChild(buy_date);
		tr[i].appendChild(lent_date);
		tr[i].appendChild(pc_brand);
		tr[i].appendChild(pc_model);
		tr[i].appendChild(repair_id);
		tr[i].appendChild(describe);
		tr[i].appendChild(repair_status);
		tr[i].appendChild(details);
		tr[i].appendChild(change_stat);
	}
}
function repairdeal_show_details(x)
{
 	alert("cpu:    " + main_json[x].pc_cpu + "\n" + "内存:   " + main_json[x].pc_ram + "\n" + "硬盘:   " + main_json[x].pc_hdd + "\n" + "显卡:   " + main_json[x].pc_gpu  + "\n sn:   " + main_json[x].pc_sn + "\n" + "描述:   " + main_json[x].describe + "\n");
}

function repairdeal_now_deal(x)
{
	$.post("repairdeal.php", {repair_id: x}, function(response_text, status){
   		var zz = /success/i;
		if (zz.test(response_text))
			swal(response_text, "已处理！", "success");
		else
			swal(response_text, "处理失败，请检查网络连接！", "error");
  	});
}