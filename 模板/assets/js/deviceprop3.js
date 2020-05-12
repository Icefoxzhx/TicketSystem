$(function(){
	$.get("deviceprop3.php", function(data,status){
		// alert(data);
		if(data == "PC404")
			main_json = [{"total_num":0}];
		else
		{
			raw_json = data;
			main_json = JSON.parse(raw_json);
			t = main_json.length;
			var fir = {"total_num" : t};
			main_json.unshift(fir);
		}
		deviceprop3_show_total();
		deviceprop3_write_table(main_json[0].total_num);
	});
});
function deviceprop3_show_total()
{
//	alert("dzsdfaef");
	document.getElementById("deviceprop3_total").innerHTML = main_json[0].total_num;
}
function deviceprop3_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr = document.createElement("tr");
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
		details.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=deviceprop3_show_details(' + i + ');>详情</button>';
		var tab=document.getElementById("deviceprop3_main_table");
		tab.appendChild(tr);
		tr.appendChild(num);
		tr.appendChild(id);
		tr.appendChild(owner);
		tr.appendChild(user);
		tr.appendChild(buy_date);
		tr.appendChild(lent_date);
		tr.appendChild(sn);
		tr.appendChild(pc_brand);
		tr.appendChild(pc_model);
		tr.appendChild(details);
	}
}
function deviceprop3_show_details(x)
{
	alert("cpu:    " + main_json[x].pc_cpu + "\n" + "内存:   " + main_json[x].pc_ram + "\n" + "硬盘:   " + main_json[x].pc_hdd + "\n" + "显卡:   " + main_json[x].pc_gpu + "\n");
}