$(function(){
	$.get("deviceprop3.php", function(data,status){
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
		document.getElementById('deviceprop_total_num').innerHTML = main_json[0].total_num;
	});
});