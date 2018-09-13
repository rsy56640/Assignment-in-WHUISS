<%@ page language="java" pageEncoding="UTF-8"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
</head>
<style type="text/css">
<!--
.border {
	color: blue;
	border-width: 2px 2px 2px 2px;
	border-color: #ac42ff;
	border-style: inset;
}

.button {
	background-color: #4CAF50; /* Green */
	border: none;
	color: white;
	padding: 15px 32px;
	text-align: center;
	s text-decoration: none;
	display: inline-block;
	font-size: 16px;
}

.button2 {
	background-color: #008CBA;
	color: black;
	border: 2px solid #008CBA;
}

body {
	background-image: url('images/_imgbankbg2.jpg');
	background-size: 1650px 732px;
	background-repeat: no-repeat;
	/*background-size: cover;*/
}

img.right_top {
	position: absolute;
	top: 0px;
	right: 0px;
	width: 100px;
	height: 100px;
}

img.right_bottom {
	position: absolute;
	bottom: 0px;
	right: 0px;
	width: 100px;
	height: 100px;
}

img.left_bottom {
	position: absolute;
	bottom: 0px;
	left: 0px;
	width: 100px;
	height: 100px;
}
-->
</style>

<!-- 
		background-size: cover;
		background-size: 1650px 732px;
 -->

<body>

	<img src="images/_imgback.png" class="right_top" />
	<img src="images/_imgback.png" class="right_bottom" />
	<img src="images/_imgbank.png" class="left_bottom" />

	<div align='center'>
		<h1>Welcome to BANK SYSTEM</h1>

		<br>

		<form action="hello" method="post">
			<table align='center'>
				<tr>
					<td>ID</td>
					<td><input type="text" style="width: 160px; height: 25px"
						name="id"></td>
				</tr>
				<tr>
					<td>姓名</td>
					<td><input type="text" style="width: 160px; height: 25px"
						name="name"></td>
				</tr>
				<tr>
					<td>密码</td>
					<td><input type="password" style="width: 160px; height: 25px"
						name="password"></td>
					<!-- <textarea type="text" style="display: none" name="id">1</textarea> -->
				</tr>
			</table>
			<input type="submit" value="登陆" class="button">
		</form>
	</div>

	<br>
	<br>
	<br>
	<br>
	<br>

	<div align='center'>
		<table width=30% border=0>
			<tr>
				<td>
					<p align='center' class=border>
						<a href="./register.jsp">没有账号，注册</a>
					</p>
				</td>
				<td width="50">&emsp;&emsp;&emsp;</td>
				<td>
					<form action="BackStage" method="post">
						<input type="submit" value="后台" class="button button2">
					</form>
				</td>
			</tr>
		</table>
	</div>


</body>
</html>