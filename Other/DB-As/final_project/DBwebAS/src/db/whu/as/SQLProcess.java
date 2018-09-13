package db.whu.as;

import java.util.*;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;

public class SQLProcess {

	// type = 0 : system -- allow delete
	// type = 1 : cilent -- no deletion
	public static void process(ResultSet rs, PrintWriter pr, int type, int id) {
		if (rs == null) {
			pr.print("<div align = \"center\">" + "<h2>sql execution fails!</h2>" + "</div>");
			return;
		}

		String action = "";
		if (type == 0)
			action = action + "Update";
		if (type == 1)
			action = action + "Client";
		String form_start = "<form action=\"" + action + "\" method=\"post\">";
		if (type == 1) {
			String ID = "" + id;
			form_start = form_start + "<textarea type=\"text\" style=\"display: none\" name=\"id\">" + ID
					+ "</textarea>";
		}

		pr.print("<div align = \"center\">");
		pr.print(form_start);

		// adjust the position according to the background image
		pr.println("<br><br><br><br><br><br>");

		pr.print("<table border = '2' cellpadding = '12'>");
		// record the column
		int index = 0;
		try {
			final int col = rs.getMetaData().getColumnCount();
			pr.print("<tr>");
			for (int i = 1; i <= col; i++) {
				pr.print("<td>");
				pr.print(rs.getMetaData().getColumnName(i) + " ");
				pr.print("</td>");
			}
			// add an option
			String option = "<select name = \"type\">" + "<option value =\"deposit\">deposit</option>"
					+ "<option value =\"draw\">draw</option>" + "</select>";
			pr.print("<td>");
			pr.print(option);
			pr.print("</td>");

			// delete checkbox column
			if (type == 0) {
				pr.print("<td>");
				pr.print("DELETE");
				pr.print("</td>");
			}

			pr.print("</tr>");
			// display info
			while (rs.next()) {
				index++;
				pr.print("<tr>");
				for (int i = 1; i <= col; i++) {
					pr.print("<td>");
					pr.print(rs.getString(i) + " ");
					pr.print("</td>");
				}

				// add ID value
				String _ID = rs.getString(1);
				String id_val = "<textarea type=\"text\" style=\"display: none\" name=\"" + index + "id\">" + _ID
						+ "</textarea>";
				pr.print(id_val);

				// input deposit/draw amount
				String text = "<input type=\"text\" style=\"width: 120px; height: 25px\" name=\"" + index + "row\">";
				pr.print("<td>");
				pr.print(text);
				pr.print("</td>");

				// choose whether to delete
				if (type == 0) {
					String delete = "<input type=\"checkbox\" name=\"" + index + "delete\" value=\"" + rs.getString(1)
							+ "\" checked=\"\"></input>";
					pr.print("<td>");
					pr.print(delete);
					pr.print("</td>");
				}

				pr.print("</tr>");
			}
		} catch (SQLException e1) {
			System.out.println("ResultSet读取失败！");
			e1.printStackTrace();
		}

		pr.print("</table>");
		pr.print("<br><br>");
		// add a button to submit index
		String form_end = "<textarea type=\"text\" style=\"display: none\" name=\"colNum\">" + index + "</textarea>"
				+ "<input type=\"submit\" value=\"submit\" class=\"button button2\"> " + "</form>";
		pr.print(form_end);
		pr.println("</div>");

		if (rs != null) {
			try {
				rs.close();
			} catch (SQLException e) {
				System.out.println("关闭ResultSet对象失败！");
				e.printStackTrace();
			}
		}

	}
}
