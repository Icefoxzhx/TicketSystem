var main_json = [{"total_num" : 0}];
$(function(){
	$("#transfind1_inquire").ajaxForm(function(response_text)
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
		transfind1_show_total();
		transfind1_write_table(main_json[0].total_num);
	 });
});
function refresh_main_table()
{
	for(var i = 1; i <= main_json[0].total_num; i++)
		tab.removeChild(tr[i]);
}

function transfind1_show_total()
{
	document.getElementById('transfind1_total').innerHTML = '总共查询到<b>' + main_json[0].total_num +'</b>条记录';
}
var tab =document.getElementById("transfind1_main_table");
var tr = new Array();
tr[1] = 'abc';
function transfind1_write_table(m)
{
	for(var i = 1; i <= m; i++)
	{
		tr[i] = document.createElement("tr");
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
		// details_button.setAttribute("class", "btn btn-link transfind1_show_details_button");
		// details_button.setAttribute("style", "font-size:12px");
		// details_button.setAttribute("onxlick", "font-size:12px");
		// details_button.setAttribute("id", i);
		// text_details_button = document.createTextNode("详情");
		details.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=transfind1_show_details(' + i + ');>详情</button>';
		tab.appendChild(tr[i]);
		tr[i].appendChild(num);
		tr[i].appendChild(id);
		tr[i].appendChild(date);
		tr[i].appendChild(tg_id);
		tr[i].appendChild(pc_brand);
		tr[i].appendChild(pc_model);
		tr[i].appendChild(pc_sn);
		tr[i].appendChild(from);
		tr[i].appendChild(to);
		tr[i].appendChild(prop);
		tr[i].appendChild(details);
	}
}
function transfind1_show_details(x)
{
 	alert("cpu:    " + main_json[x].pc_cpu + "\n" + "内存:   " + main_json[x].pc_ram + "\n" + "硬盘:   " + main_json[x].pc_hdd + "\n" + "显卡:   " + main_json[x].pc_gpu + "\n" + "描述:   " + main_json[x].describe + "\n");
}