<%@ page language="java" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
</head>
<body>


	<h1>后台系统</h1>

	<div align='center'>
		<h1>Welcome to BackStage</h1>

		<form action="BackStage" method="post">
			<input type="text" style="width: 160px; height: 25px" name="sql">
			<textarea type="text" style="display: none" name="id" value=""></textarea>
			<input type="submit" value="execute">
		</form>
	</div>

</body>
</html>