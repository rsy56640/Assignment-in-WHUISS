package db.whu.as;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.swing.JOptionPane;

public class MyServlet extends HttpServlet {
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("<html>");
		out.println("<title>BANK SYSTEM</title>");
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
		out.println("<img src=\"images/deposit.png\" class =\"right_bottom\"/>"
				+ "<img src=\"images/deposit.png\" class =\"left_bottom\"/>");

		String id = request.getParameter("id");
		String name = request.getParameter("name");
		String password = request.getParameter("password");
		if (id == null || id.equals("")) { // why not no response???
			// System.out.println("qwe" + id + "qwe");
			out.println("<script language='javascript'>alert('error with id, username or password');"
					+ "window.location.href='./index.jsp';</script>");
		}

		DBConnector c = new DBConnector();
		c.getConnection();

		// verify Identification
		String verify = "SELECT name, password FROM BANK WHERE id = " + id;
		ResultSet rs = c.sqlOperation(verify);
		boolean validate = true, RS = true;
		try {
			if (rs == null) {
				RS = false;
			}

			if (!RS || !rs.next() || !rs.getString(1).equals(name) || !rs.getString(2).equals(password)) {
				// System.out.println(rs.getString(1) + " " + rs.getString(2));
				// System.out.println(name + " " + password);
				validate = false;
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			// e.printStackTrace();
			validate = false;
			System.out.println(id);
		}
		if (!RS) { // WTF why not no response???
			c.closed();
			out.println("<script language='javascript'>alert('error with SQL operation');"
					+ "window.location.href='./index.jsp';</script>");
		}
		if (!validate) {
			c.closed();
			// System.out.println(RS);
			out.println("<script language='javascript'>alert('error with id, username or password');"
					+ "window.location.href='./index.jsp';</script>");
			// response.sendRedirect("./index.jsp");
		} else {

			String sql = "SELECT * FROM BANK WHERE id = " + id;
			SQLProcess.process(c.sqlOperation(sql), out, 1, Integer.parseInt(id));

			out.print("<br><br>");
			out.println("<div align=\"center\">" + "<a href='./index.jsp'>back homepage</a>" + "</div>");
			out.println("</body>");

			String unchecked = "<script>var checkboxes = document.getElementsByTagName('input');"
					+ "for (var i=0; i<checkboxes.length; i++)  {" + " if (checkboxes[i].type == \"checkbox\") {"
					+ "checkboxes[i].checked = false;" + "}" + "} </script>";
			out.print(unchecked);

			out.println("</html>");
			c.closed();
		}

	}

}