var sidebarele = document.getElementsByClassName('sidebar');
sidebarele[0].innerHTML = '\
<div class="nano">\
    <div class="nano-content">\
        <div class="logo">\
            <a href="index.html">\<img src="assets/images/logo.png" alt="logo" width=60 height=60></a>\
        </div>\
        <ul>\
        	<li><a href="index.html"><i class="ti-home"></i> 概况 </a></li>\
        	<li><a class="sidebar-sub-toggle"><i class="ti-bar-chart-alt"></i> 车次操作 <span class="sidebar-collapse-icon ti-angle-down"></span></a>\
        	<ul>\
        		<li><a href="trainadd.html"> 添加车次 </a></li>\
                <li><a href="traininquire.html"> 查询车次 </a></li>\
                <li><a href="trainrelease.html"> 发布车次 </a></li>\
                <li><a href="traindelete.html"> 删除车次 </a></li>\
        	</ul>\
        </li>\
        <li><a class="sidebar-sub-toggle"><i class="ti-layout"></i> 用户管理 <span class="sidebar-collapse-icon ti-angle-down"></span></a>\
        	<ul>\
        		<li><a href="userman.html"> 用户信息查询修改 </a></li>\
                <li><a href="useradd.html"> 添加新用户 </a></li>\
        	</ul>\
        </li>\
        <li><a href="logout.php"><i class="ti-close"></i> 注销 </a></li>\
        <li><a href="clean.php"><i class="ti-close"></i> 一键删库 </a></li>\
    </div>\
</div>';