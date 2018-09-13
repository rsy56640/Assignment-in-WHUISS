package db.whu.as;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class BackStage extends HttpServlet {

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

		DBConnector c = new DBConnector();// class name
		c.getConnection();

		String trigger = "CREATE OR REPLACE TRIGGER Bound_Check BEFORE UPDATE ON BANK FOR EACH ROW"
				+ "BEGIN IF(:new.deposit < 0) THEN :new.deposit := 0; END IF; END Bound_Check;";
		c.sqlExecute(trigger);

		String procedure = "CREATE OR REPLACE PROCEDURE total(num out int) AS " + "v_total BANK.deposit%TYPE;"
				+ "v_deposit BANK.deposit%TYPE;" + "BEGIN SELECT SUM(deposit) INTO num FROM BANK; END total;";
		c.sqlExecute(procedure);

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
