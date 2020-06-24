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
			document.getElementById('float-right').innerHTML = '<div class="header-icon user-avatar">欢迎您，' + getCookie("username") +'</div>';
		else
			document.getElementById('float-right').innerHTML = '\
			<a class="user-avatar header-icon" id="login" href="login.html">&nbsp&nbsp&nbsp&nbsp&nbsp登录</a>\
            <a class="user-avatar header-icon" id="register" href="register.html">&nbsp&nbsp&nbsp&nbsp&nbsp注册</a>';
});

// <div class="dropdown dib">
//                             <div class="header-icon" data-toggle="dropdown">
//                                 <span class="user-avatar" id="user-avatar"></span>
//                                 <i class="ti-angle-down f-s-10"></i>
//                                 <div class="drop-down dropdown-profile dropdown-menu dropdown-menu-right">
//                                     <div class="dropdown-content-body">
//                                         <ul>
//                                             <li>
//                                                 <a href="userman.html">
//                                                     <i class="ti-user"></i>
//                                                     <span href="userman.html">用户资料</span>
//                                                 </a>
//                                             </li>
//                                             <li>
//                                                 <a href="logout.php">
//                                                     <i class="ti-power-off"></i>
//                                                     <span href="logout.php">注销</span>
//                                                 </a>
//                                             </li>
//                                         </ul>
//                                     </div>
//                                 </div>
//                             </div>
//                         </div>
                        
//                     </div>