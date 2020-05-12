$(function(){
	$.get("repairlookup3.php", function(data,status){
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
		repairlookup3_show_total();
		repairlookup3_write_table(main_json[0].total_num);
	});
});
function repairlookup3_show_total()
{
//	alert("dzsdfaef");
	document.getElementById("repairlookup3_total").innerHTML = main_json[0].total_num;
}
function repairlookup3_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr = document.createElement("tr");
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
		date = document.createElement("td");
		date.innerHTML = main_json[i].date;
		pc_brand = document.createElement("td");
		pc_brand.innerHTML = main_json[i].pc_brand;
		pc_model = document.createElement("td");
		pc_model.innerHTML = main_json[i].pc_model;
		// repair_id = document.createElement("td");
		// repair_id.innerHTML = main_json[i].repair_id;
		id = document.createElement("td");
		id.innerHTML = main_json[i].id;
		describe = document.createElement("td");
		describe.innerHTML = main_json[i].describe;
		repair_status = document.createElement("td");
		repair_status.innerHTML = main_json[i].repair_status;
		details = document.createElement("td");
		//details_button = document.createElement("button");
		// details_button.setAttribute("class", "btn btn-link repairlookup3_show_details_button");
		// details_button.setAttribute("style", "font-size:12px");
		// details_button.setAttribute("onxlick", "font-size:12px");
		// details_button.setAttribute("id", i);
		// text_details_button = document.createTextNode("详情");
		details.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=repairlookup3_show_details(' + i + ');>详情</button>';
		var tab=document.getElementById("repairlookup3_main_table");
		tab.appendChild(tr);
		tr.appendChild(num);
		tr.appendChild(tg_id);
		tr.appendChild(owner);
		tr.appendChild(user);
		tr.appendChild(buy_date);
		tr.appendChild(lent_date);
		tr.appendChild(date);
		tr.appendChild(pc_brand);
		tr.appendChild(pc_model);
		// tr.appendChild(repair_id);
		tr.appendChild(id);
		tr.appendChild(describe);
		tr.appendChild(repair_status);
		tr.appendChild(details);
	}
}
function repairlookup3_show_details(x)
{
 	alert("cpu:    " + main_json[x].pc_cpu + "\n" + "内存:   " + main_json[x].pc_ram + "\n" + "硬盘:   " + main_json[x].pc_hdd + "\n" + "显卡:   " + main_json[x].pc_gpu  + "\n sn:   " + main_json[x].pc_sn + "\n" + "描述:   " + main_json[x].describe + "\n");
}