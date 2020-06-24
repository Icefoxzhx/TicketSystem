//------------时间线部分-------------
var timeline_badge_category = new Array("primary", "warning", "danger", "success", "default", "info");
function append_on_timeline(m)
{
	var timeline_node_content = '\
		<li id="main_timeline_node_'+m+'">\
            <div class="timeline-badge '+timeline_badge_category[m%6]+'">'+m+'</div>\
            <div class="timeline-panel">\
                <div class="timeline-heading">\
                	<div class="input-group">\
                		<span class="input-group-text" style="width:130px;">站名</span>\
                    	<input class="form-control input-default add_stations" type="text" name="add_stations_'+m+'" id="add_stations_'+m+'" required="required">\
                	</div>';
    if(m != 1)
    {
    	timeline_node_content += '\
    				<div class="input-group">\
                		<span class="input-group-text" style="width:130px; font-size:14px">上站到此站价格</span>\
                    	<input class="form-control input-default add_prices" type="number" min="0" name="add_prices_'+m+'" id="add_prices_'+m+'" required="required">\
                	</div>\
                	<div class="input-group">\
                		<span class="input-group-text" style="width:130px; font-size:14px">上站到此站历时</span>\
                    	<input class="form-control input-default add_traveltimes" type="number" min="0" name="add_traveltimes_'+m+'" id="add_traveltimes_'+m+'" required="required">\
                		<span class="input-group-text">分钟</span>\
                	</div>';
    }
    if(m != 1 && m != main_timeline_cnt)
    {
    	timeline_node_content += '\
                	<div class="input-group">\
                		<span class="input-group-text" style="width:130px; font-size:14px">本站停靠时间</span>\
                		<input class="form-control input-default add_stopovertimes" type="number" name="add_stopoverTimes_'+m+'" id="add_stopoverTimes_'+m+'" min="0" max="10000" required="required">\
                		<span class="input-group-text">分钟</span>\
                	</div>';
    timeline_node_content += '\
                </div>\
            </div>\
        </li>';
	}
                                    
	$("#main_timeline").append(timeline_node_content);
}
function remove_on_timeline(m)
{
	$("#main_timeline_node_"+m).remove();
}

var main_timeline_cnt = 2;
append_on_timeline(1);
append_on_timeline(2);
$(function(){
	$("#add_stationnum").change(function(){
		main_timeline_cnt =  parseInt(main_timeline_cnt);
		if($(this).val() < 2)
		{
			swal("越界！","只能是2-100的整数","error");
			$(this).val('2');
		}
		else if($(this).val() > 100)
		{
			swal("越界！","只能是2-100的整数","error");
			$(this).val('100');
		}
		var pre_timeline_cnt = main_timeline_cnt;
		main_timeline_cnt = parseInt($(this).val());
		if(pre_timeline_cnt < main_timeline_cnt)
		{
			remove_on_timeline(pre_timeline_cnt);
			append_on_timeline(pre_timeline_cnt);
			for(var i = pre_timeline_cnt + 1; i <= main_timeline_cnt; i++)
				append_on_timeline(i);
		}
		else if(pre_timeline_cnt > main_timeline_cnt)
		{
			for(var i = pre_timeline_cnt; i > main_timeline_cnt; i--)
				remove_on_timeline(i);
			remove_on_timeline(main_timeline_cnt);
			append_on_timeline(main_timeline_cnt);
		}
	});
});


$(function(){
	$("#add_trainid").change(function(){
		document.getElementById("timeline_trainid").innerHTML = "火车编号：" + $(this).val();
	});
});


//------------表单控制-------------
function array_to_str(tar_arr)
{
	var tar_str = "";
	for(var i = 0; i < tar_arr.length; i++)
	{
		tar_str += tar_arr[i].value;
		if(i != tar_arr.length - 1)
			tar_str += "|";
	}
	return tar_str;
}
$(function(){
	$("#trainadd_button").click(function(){
		var check_input_array = document.getElementsByClassName("form-control");
		for(var i = 0; i < check_input_array.length; i++)
		{
			console.log(check_input_array[i].value);
			if(check_input_array[i].value == "")
			{
				swal("信息没有填写完整！", check_input_array[i].name+"不能为空！", "error");
				check_input_array[i].classList.add("input-focus");
				return false;
			}
		}
		if($("#add_saledate_from").val() < "2020-06-01")
		{
			swal("信息填写错误！", "日期错误！", "error");
			$("#add_saledate_from").val("2020-06-01");
			$("#add_saledate_from").addClass("input-focus");
			return false;
		}
		if($("#add_saledate_to").val() > "2020-08-31" || $("#add_saledate_to").val() < $("#add_saledate_from").val())
		{
			swal("信息填写错误！", "日期错误！", "error");
			$("#add_saledate_to").val("2020-08-31");
			$("#add_saledate_to").addClass("input-focus");
			return false;
		}
		var add_stations_array = document.getElementsByClassName('add_stations');
		var add_prices_array = document.getElementsByClassName('add_prices');
		var add_traveltimes_array = document.getElementsByClassName('add_traveltimes');
		var add_stopovertimes_array = document.getElementsByClassName('add_stopovertimes');
		$.ajax({
	        type: "POST",
	        dataType: "text",
	        url: "trainadd.php", 
	        data: {
	        	add_trainid: $("#add_trainid").val(),
	        	add_seatnum: $("#add_seatnum").val(),
	        	add_stationnum: $("#add_stationnum").val(),
	        	add_stations: array_to_str(add_stations_array),
	        	add_prices: array_to_str(add_prices_array),
	        	add_starttime: $("#add_starttime_h").val() + ":" + $("#add_starttime_m").val(),
	        	add_traveltimes: array_to_str(add_traveltimes_array),
	        	add_stopovertimes: array_to_str(add_stopovertimes_array),
	        	add_saledate: $("#add_saledate_from").val().substring(5) + "|" + $("#add_saledate_to").val().substring(5),
	        	add_type: $("#add_type").val(),

	        },
	        success: function (response_text_text) {
				if(response_text_text == "-1")
					swal("Oops", "添加车次失败，可能是已经填加过同名车次，请检查输入", "error");
				else if(response_text_text == "0")
					swal("Success", "添加车次成功！", "success");
	        },
	    });
	});
});

$(function(){
	$("#add_starttime_h").change(function(){
		if($("#add_starttime_h").val() <= 9)
			$("#add_starttime_h").val("0" + $("#add_starttime_h").val());
	});
	$("#add_starttime_m").change(function(){
		if($("#add_starttime_m").val() <= 9)
			$("#add_starttime_m").val("0" + $("#add_starttime_m").val());
	});
});