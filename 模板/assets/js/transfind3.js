$(function(){
	$.get("transfind3.php", function(data,status){
	//	alert(data);
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
		transfind3_show_total();
		transfind3_write_table(main_json[0].total_num);
	});
});
function transfind3_show_total()
{
//	alert("dzsdfaef");
	document.getElementById("transfind3_total").innerHTML = main_json[0].total_num;
}
function transfind3_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr = document.createElement("tr");
		num = document.createElement("td");
		num.innerHTML = i;
		id = document.createElement("td");
		id.innerHTML = main_json[i].id;
		date = document.createElement("td");
		date.innerHTML = main_json[i].date;
		tg_id = document.createElement("td");
		tg_id.innerHTML = main_json[i].tg_id;
		pc_brand = document.createElement("td");
		pc_brand.innerHTML = main_json[i].pc_brand;
		pc_model = document.createElement("td");
		pc_model.innerHTML = main_json[i].pc_model;
		pc_sn = document.createElement("td");
		pc_sn.innerHTML = main_json[i].pc_sn;
		from = document.createElement("td");
		from.innerHTML = main_json[i].from_id;
		to = document.createElement("td");
		to.innerHTML = main_json[i].to_id;
		prop = document.createElement("td");
		if(main_json[i].prop_type == 1)
			prop.innerHTML = "拥有权";
		else if(main_json[i].prop_type == 2)
			prop.innerHTML = "使用权";
		details = document.createElement("td");
		//details_button = document.createElement("button");
		// details_button.setAttribute("class", "btn btn-link transfind3_show_details_button");
		// details_button.setAttribute("style", "font-size:12px");
		// details_button.setAttribute("onxlick", "font-size:12px");
		// details_button.setAttribute("id", i);
		// text_details_button = document.createTextNode("详情");
		details.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=transfind3_show_details(' + i + ');>详情</button>';
		var tab=document.getElementById("transfind3_main_table");
		tab.appendChild(tr);
		tr.appendChild(num);
		tr.appendChild(id);
		tr.appendChild(date);
		tr.appendChild(tg_id);
		tr.appendChild(pc_brand);
		tr.appendChild(pc_model);
		tr.appendChild(pc_sn);
		tr.appendChild(from);
		tr.appendChild(to);
		tr.appendChild(prop);
		tr.appendChild(details);
	}
}
function transfind3_show_details(x)
{
 	alert("cpu:    " + main_json[x].pc_cpu + "\n" + "内存:   " + main_json[x].pc_ram + "\n" + "硬盘:   " + main_json[x].pc_hdd + "\n" + "显卡:   " + main_json[x].pc_gpu + "\n" + "描述:   " + main_json[x].describe + "\n");
}