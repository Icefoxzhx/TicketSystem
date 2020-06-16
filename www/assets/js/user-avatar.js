function getCookie(cname)
{
	var name = cname + "=";
	var ca = document.cookie.split(';');
	for(var i = 0; i<ca.length; i++) 
	{
		var c = ca[i].trim();
		if (c.indexOf(name)==0)
			return c.substring(name.length,c.length);
	}
	return "";
}
$(function(){
		if(getCookie("username"))
			document.getElementById('user-avatar').innerHTML = "欢迎您，" + getCookie("username");
		else
			document.getElementById('user-avatar').innerHTML = "欢迎您";
});