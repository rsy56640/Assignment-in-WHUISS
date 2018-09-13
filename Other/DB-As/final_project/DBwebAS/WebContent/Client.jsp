<%@ page language="java"
	import="java.util.*,java.io.*,java.sql.*,db.whu.as.*;"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Insert title here</title>
</head>
<body>
	<%
		out.println("<html><body>");
		out.println("<h1 style='text-align:center'>Welcome to BankSystem</h1>");

		String id = request.getParameter("id");
		String name = request.getParameter("name");
		String password = request.getParameter("password");
		if (id == null) {
			out.println("<script>alert(\"ID Exception\");</script>");
			response.sendRedirect("./index.jsp");
		}

		DBConnector c = new DBConnector();// class name
		c.getConnection();

		// verify Identification
		String verify = "SELECT name, password FROM BANK WHERE id = " + id;
		ResultSet rs = c.sqlOperation(verify);
		boolean validate = true;
		try {
			if (!rs.next() || !rs.getString(1).equals(name) || !rs.getString(2).equals(password))
				validate = false;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			//e.printStackTrace();
		}
		if (!validate) {
			c.closed();
			out.println("<script>alert(\"error with id, username or password\");</script>");
			response.sendRedirect("./index.jsp");
		}

		PrintWriter pwout = response.getWriter();
		String sql = "SELECT * FROM BANK WHERE id = " + id;
		SQLProcess.process(c.sqlOperation(sql), pwout, 1, Integer.parseInt(id));

		out.println("</body></html>");
		c.closed();
	%>

</body>
</html>