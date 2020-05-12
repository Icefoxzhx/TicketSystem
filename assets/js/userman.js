var main_json = [{"total_num" : 0}];
$(function(){
	$("#userman_inquire").ajaxForm(function(response_text)
	{
		alert(response_text);
		refresh_main_table();
		if(response_text == "no")
			main_json = [{"total_num": 0}];
		else
		{
			$("#card_inquire").show();
			raw_json = response_text;
			main_json = JSON.parse(raw_json);
			t = main_json.length;
			var fir = {"total_num" : t};
			main_json.unshift(fir);
		}
		userman_show_total();
		userman_write_table(main_json[0].total_num);
	 });
});
$(function(){
	$("#modify_profile").ajaxForm(function(response_text)
	{
		$("#card_modify_profile").hide();
		$("#card_inquire").hide();
		$("#userman_total").hide();
		alert(response_text);
		var main_json = [{"total_num" : 0}];
	});
});
function refresh_main_table()
{
	for(var i = 1; i <= main_json[0].total_num; i++)
		tab.removeChild(tr[i]);
}

function userman_show_total()
{
	$("#userman_total").show();
	document.getElementById('userman_total').innerHTML = '总共查询到<b>' + main_json[0].total_num +'</b>名用户';
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
		details.innerHTML = '<button class="btn btn-link" style="font-size:12px" onclick=userman_choose(' + i + ');>修改</button>';
		tab.appendChild(tr[i]);
		tr[i].appendChild(num);
		tr[i].appendChild(usernamee);
		tr[i].appendChild(namee);
		tr[i].appendChild(mailAddrr);
		tr[i].appendChild(privilege);
		tr[i].appendChild(details);
	}
}

$(function(){
	$("#card_inquire").hide();
	$("#card_modify_profile").hide();
	$("#userman_total").hide();
});

function userman_choose(x)
{
	$("#card_modify_profile").show();
	document.getElementById('modify_username').value = main_json[x].username;
	document.getElementById('modify_name').value = main_json[x].name;
	document.getElementById('modify_mailAddr').value = main_json[x].mailAddr;
	document.getElementById('modify_privilege').value = main_json[x].privilege;
}