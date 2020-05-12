var main_json = [{"total_num" : 0}];
var ckt = 0;
$(function(){
	$.get("deviceprop3.php", function(data,status)
	{
	//	alert(response_text);
		refresh_main_table();
		if(data == "PC404")
			main_json = [{"total_num": 0}];
		else
		{
	//		alert(response_text);
			raw_json = data;
			main_json = JSON.parse(raw_json);
			t = main_json.length;
			var fir = {"total_num" : t};
			main_json.unshift(fir);
		}
	//	repairadd_deviceprop_show_total();
		repairadd_deviceprop_write_table(main_json[0].total_num);
	 });
});

// function http()
// {
// 	var req;
// 	req = new XMLHttpRequest();
// 	req.onreadystatechange=function()
// 	{
// 		if (req.readyState==4 && req.status==200)
// 		{
// 			raw_json = req.responseText;
// 			main_json = JSON.parse(raw_json);
// 		}
// 	}
// 	req.open("GET","deviceprop1.php",false);
// 	req.send();
// }
function refresh_main_table()
{
	for(var i = 1; i <= main_json[0].total_num; i++)
		tab.removeChild(tr[i]);
}


//function repairadd_deviceprop_show_total()
//{
//	document.getElementById('repairadd_deviceprop_total').innerHTML = '总共查询到<b>' + main_json[0].total_num +'</b>台电脑';
//}


var tab =document.getElementById("repairadd_deviceprop_table");
var tr = new Array();
tr[1] = 'abc';
function repairadd_deviceprop_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr[i] = document.createElement("tr");
		num = document.createElement("td");
		num.innerHTML = i;
		id = document.createElement("td");
		id.innerHTML = main_json[i].id;
		owner = document.createElement("td");
		owner.innerHTML = main_json[i].owner_id;
		user = document.createElement("td");
		user.innerHTML = main_json[i].user_id;
		buy_date = document.createElement("td");
		buy_date.innerHTML = main_json[i].buy_date;
		lent_date = document.createElement("td");
		lent_date.innerHTML = main_json[i].lent_date;
		sn = document.createElement("td");
		sn.innerHTML = main_json[i].pc_sn;
		pc_brand = document.createElement("td");
		pc_brand.innerHTML = main_json[i].pc_brand;
		pc_model = document.createElement("td");
		pc_model.innerHTML = main_json[i].pc_model;
		details = document.createElement("td");
		details.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=repairadd_deviceprop_show_details(' + i + ');>详情</button>';
		choose = document.createElement("td");
		choose.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=repairadd_choose(' + i + ');>选中</button>';
		tab.appendChild(tr[i]);
		tr[i].appendChild(num);
		tr[i].appendChild(id);
		tr[i].appendChild(owner);
		tr[i].appendChild(user);
		tr[i].appendChild(buy_date);
		tr[i].appendChild(lent_date);
		tr[i].appendChild(sn);
		tr[i].appendChild(pc_brand);
		tr[i].appendChild(pc_model);
		tr[i].appendChild(details);
		tr[i].appendChild(choose);
	}
}


function repairadd_deviceprop_show_details(x)
{
	alert("cpu:    " + main_json[x].pc_cpu + "\n" + "内存:   " + main_json[x].pc_ram + "\n" + "硬盘:   " + main_json[x].pc_hdd + "\n" + "显卡:   " + main_json[x].pc_gpu + "\n");
}

$(function(){
	$("#card_add").hide();
});

$(function(){
	$("#show_add").click(function(){
		$("#card_add").show();
	});
});
function repairadd_choose(x)
{
	$("#card_add").show();
	document.getElementById('tg_id').value = main_json[x].id;
}

// var activate_prop = 2;
// function prop_type1_listener()
// {
// 	activate_prop = 1;
// }
// function prop_type2_listener()
// {
// 	activate_prop = 2;
// }
// if(activate_prop == 1)
// {
// 	var whether3 = confirm('是否同步将使用权转交给该用户');
// 	if (whether3)
// 		activate_prop = 3;
// 	else
// 		activate_prop = 1;
// }

$(function(){
	$("#repairadd").ajaxForm(function(response_text)
	{
		var zz = /success/i;
		if (zz.test(response_text))
			swal(response_text, "维修申请添加成功！", "success");
		else
			swal(response_text, "维修申请添加失败，请检查您的输入！", "error");
	});
});