<%@ page language="java" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Register Page</title>
</head>

<style type="text/css">
<!--
.button {
	background-color: #4CAF50; /* Green */
	border: none;
	color: white;
	padding: 15px 32px;
	text-align: center;
	text-decoration: none;
	display: inline-block;
	font-size: 16px;
}

.button2 {
	background-color: #4CAF50;
	color: white;
}

body {
	background-image: url('images/_imgbankbg2.jpg');
	background-size: 1650px 732px;
	background-repeat: no-repeat;
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

<body>

	<img src="images/card.png" class="right_bottom" />
	<img src="images/card.png" class="left_bottom" />

	<div align='center'>
		<h1>Welcome to BANK SYSTEM</h1>

		<form action="Register" method="post">
			<table align='center'>
				<tr>
					<td>姓名</td>
					<td><input type="text" style="width: 160px; height: 25px"
						name="name"></td>
				</tr>
				<tr>
					<td>密码</td>
					<td><input type="password" style="width: 160px; height: 25px"
						name="password"></td>
				</tr>
			</table>
			<input type="submit" value="注册" class="button button2">
		</form>
	</div>

	<br>
	<br>

	<div align='center'>
		<a href='./index.jsp'>back homepage</a>
	</div>

</body>
</html>