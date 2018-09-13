package db.whu.as;

import java.util.*;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;

public class DBConnector {

	private Connection con;
	private String user = "system";
	// private String user = "sys as sysdba";

	private String password = "RSY19971206rsy00";
	private String className = "oracle.jdbc.driver.OracleDriver";
	// private String url="jdbc:oracle:oci@localhost:1158:orcl";//这个url可能无效
	// private String url = "jdbc:oracle:thin:@hr-PC:1521:orcl";
	// private String url = "jdbc:oracle:thin:@localhost:1521:orcl";
	private String url = "jdbc:oracle:oci:@";
	java.sql.Statement st = null;

	public DBConnector() {
		try {
			Class.forName(className);
			// System.out.println("加载数据库驱动成功！");
		} catch (ClassNotFoundException e) {
			System.out.println("加载数据库驱动失败！");
			e.printStackTrace();
		}
	}

	public void getConnection() {
		try {
			con = DriverManager.getConnection(url, user, password);
			// System.out.println("创建数据库连接成功！");
		} catch (SQLException e) {
			System.out.print(con);
			System.out.println("创建数据库连接失败！");
			con = null;
			e.printStackTrace();
		}

		try {
			st = con.createStatement();
		} catch (SQLException e) {
			System.out.println("创建stat对象失败！");
			e.printStackTrace();
		}
	}

	public void closed() {

		if (st != null) {
			try {
				st.close();
			} catch (SQLException e) {
				System.out.println("关闭stat对象失败！");
				e.printStackTrace();
			}
		}

		if (con != null) {
			try {
				con.close();
			} catch (SQLException e) {
				System.out.println("关闭con对象失败！");
				e.printStackTrace();
			}
		}
	}

	public ResultSet sqlOperation(String s) {
		String sql = s;
		ResultSet rs = null;
		try {
			rs = st.executeQuery(sql);
		} catch (SQLException e) {
			// System.out.println("执行sql语句失败！" + s);
			// e.printStackTrace();
			return null;
		}

		return rs;
	}

	public void sqlExecute(String s) {
		String sql = s;
		try {
			st.execute(sql);
		} catch (SQLException e) {
			// System.out.println("执行sql语句失败！" + s);
			// e.printStackTrace();
		}
	}

	public int sqlExecuteProcedure(String s) {
		String sql = s;
		int ans = 0;
		try {
			CallableStatement cs = con.prepareCall("{" + sql + "}");
			cs.registerOutParameter(1, Types.INTEGER);
			cs.execute();
			Object obj = cs.getObject(1);
			ans = (int) obj;
		} catch (SQLException e) {
			// System.out.println("执行sql语句失败！" + s);
			// e.printStackTrace();
			ans = 0;
		}
		return ans;
	}

	public void sqlUpdate(String s) {
		String sql = s;
		try {
			st.executeUpdate(sql);
		} catch (SQLException e) {
			System.out.println("执行sql update语句失败！");
			e.printStackTrace();
			return;
		}
	}

	public static void main(String[] args) {
		DBConnector c = new DBConnector();// class name
		c.getConnection();

		// String sq1 = "SELECT * FROM student";
		// c.sqlOperation(sq1);

		c.closed();
	}

}
