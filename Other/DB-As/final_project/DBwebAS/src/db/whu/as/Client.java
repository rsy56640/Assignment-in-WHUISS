package db.whu.as;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.ResultSet;
import java.sql.SQLException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Client extends HttpServlet {
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

		DBConnector c = new DBConnector();
		c.getConnection();
		String id = request.getParameter("id");
		// System.out.println(id);

		String type = request.getParameter("type");
		int sign = 1;
		if (type.equals("draw"))
			sign = -1;
		int money = 0;
		try {
			int input = Integer.parseInt(request.getParameter("1" + "row"));
			if (input < 0) {
				out.print("<script language='javascript'>alert('Money cannot be minus!!!');</script>");
				input = 0;
			}
			money = sign * input;
		} catch (NumberFormatException e) {
			money = 0;
			// System.out.println(money[i]);
		}

		// do update
		String update_start = "UPDATE BANK SET deposit = deposit +";
		String update_end = "WHERE ID = ";
		c.sqlUpdate(update_start + money + update_end + id);

		String sql = "SELECT * FROM BANK WHERE id = " + id;
		SQLProcess.process(c.sqlOperation(sql), out, 1, Integer.parseInt(id));

		out.print("<br><br>");
		out.println("<div align='center'>" + "<a href='./index.jsp'>back homepage</a>" + "</div>");
		out.println("</body></html>");
		c.closed();

	}

}
