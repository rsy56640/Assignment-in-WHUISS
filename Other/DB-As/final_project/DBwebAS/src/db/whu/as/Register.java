package db.whu.as;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Register extends HttpServlet {
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("<html>");
		out.println("<title>Register Page</title>");
		out.println("<h1 style='text-align:center'>Welcome to BANK SYSTEM</h1>");

		String BackGround = "_imgbankbg2.jpg";
		out.println("<style type=\"text/css\">" + "<!--"
				+ ".button {background-color: #4CAF50; /* Green */border: none;color: white;"
				+ "padding: 15px 32px;text-align: center;text-decoration: none;display: inline-block;"
				+ "font-size: 16px;}" + ".button2 {background-color: #008CBA;color: black;border: 2px solid #008CBA;}"
				+ "body {background-image: url('images/" + BackGround
				+ "');background-size: 1650px 732px;background-repeat: no-repeat;}"
				+ "img.right_bottom {position: absolute;bottom: 0px;right: 0px;width: 100px;height: 100px;}"
				+ "img.left_bottom {position: absolute;bottom: 0px;left: 0px;width: 100px;height: 100px;}" + "-->"
				+ "</style>");
		out.println("<body>");
		out.println("<img src=\"images/card.png\" class =\"right_bottom\"/>"
				+ "<img src=\"images/card.png\" class =\"left_bottom\"/>");

		String name = request.getParameter("name");
		String password = request.getParameter("password");

		DBConnector c = new DBConnector();// class name
		c.getConnection();

		if (name == null || name.equals("") || password == null || password.equals("")) {
			out.println("<script language='javascript'>alert('name and password required');"
					+ "window.location.href='./register.jsp';</script>");
		}

		String select_id = "SELECT MAX(id) FROM BANK";
		ResultSet rs = c.sqlOperation(select_id);
		try {
			rs.next();
			int id = Integer.parseInt(rs.getString(1)) + 1;
			String insert = "INSERT INTO BANK VALUES('" + id + "','" + name + "','" + password + "',0)";
			// System.out.println(insert);
			c.sqlOperation(insert);
			out.println("<script language='javascript'>alert('Register Success');</script>");
			String sql = "SELECT * FROM BANK WHERE id = " + id;
			SQLProcess.process(c.sqlOperation(sql), out, 1, id);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		out.print("<br><br>");
		out.println("<div align='center'>" + "<a href='./index.jsp'>back homepage</a>" + "</div>");
		out.println("</body></html>");
		c.closed();

	}

}
