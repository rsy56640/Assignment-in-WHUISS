package db.whu.as;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Update extends HttpServlet {
	public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		response.setContentType("text/html");
		PrintWriter out = response.getWriter();
		out.println("<html>");
		out.println("<title>BANK SYSTEM BackStage</title>");
		out.println("<h1 style='text-align:center'>Welcome to BANK SYSTEM BackStage</h1>");

		String BackGround = "_imgbankbg1.jpg";
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
		out.println("<img src=\"images/_imgCard.png\" class =\"right_bottom\"/>"
				+ "<img src=\"images/_imgCard.png\" class =\"left_bottom\"/>");

		DBConnector c = new DBConnector();
		c.getConnection();
		int col = 0;
		try {
			col = Integer.parseInt(request.getParameter("colNum"));
		} catch (NumberFormatException e) {
			;
		}
		String type = request.getParameter("type");
		int sign = 1;
		if (type.equals("draw"))
			sign = -1;
		int[] money = new int[col + 1];
		for (int i = 1; i <= col; i++) {
			try {
				int input = Integer.parseInt(request.getParameter(i + "row"));
				if (input < 0) {
					out.println("<script language='javascript'>alert('Money cannot be minus!!!');</script>");
					input = 0;
				}
				money[i] = sign * input;
			} catch (NumberFormatException e) {
				money[i] = 0;
			}
			// System.out.println(money[i]);
		}

		// do update
		// System.out.println("执行sql update语句！" + col);
		String update_start = "UPDATE BANK SET deposit = deposit +";
		String update_end = "WHERE ID = ";
		for (int i = 1; i <= col; i++) {
			c.sqlUpdate(update_start + money[i] + update_end + request.getParameter(i + "id"));
		}

		String delete_start = "DELETE FROM BANK WHERE id=";

		boolean[] delete = new boolean[col + 1];
		for (int i = 1; i <= col; i++) {
			delete[i] = (request.getParameter(i + "delete") != null) ? true : false;
			if (delete[i])
				c.sqlOperation(delete_start + request.getParameter(i + "delete"));
		}

		String sql = "SELECT * FROM BANK ORDER BY id ASC";
		SQLProcess.process(c.sqlOperation(sql), out, 0, -1);

		out.println("<div align=\"center\">");
		int total = c.sqlExecuteProcedure("CALL total(?)");
		out.print("<h2><br>The total deposit is: " + total + "<br></h2>");
		out.print("</div>");

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
