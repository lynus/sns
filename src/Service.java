package com.edu.swu;
test new branch!!!
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import com.csvreader.CsvReader;
import com.edu.swu.gra.Edge;
import com.edu.swu.gra.FR;
import com.edu.swu.gra.Node;

public class Service {
	public static void main(String[] args) {
		CsvReader cr;

		ServerSocket ss = null;
		Socket sk = null;
		BufferedReader br = null;
		FR fr = new FR();
		OutputStream os = null;
		try {
			Random r = new Random();
			cr = new CsvReader(new FileReader(new File("file/nodes.csv")));
			int count = 0;
			while (cr.readRecord()) {
				// System.out.println(cr.getValues()[0]);
				count++;
			}
			count++;
			count++;
			Node[] nodes = new Node[count];
			cr.close();
			for (int i = 0; i < nodes.length; i++) {
				nodes[i] = new Node();
				nodes[i].id = i;
				nodes[i].x = r.nextDouble() * 100000;
				nodes[i].y = r.nextDouble() * 100000;
			}

			cr = new CsvReader(new FileReader(new File("file/edges.csv")));
			count = 0;
			while (cr.readRecord()) {
				count++;
			}
			Edge[] edges = new Edge[count];
			count = 0;
			cr.close();
			cr = new CsvReader(new FileReader(new File("file/edges.csv")));
			while (cr.readRecord()) {
				edges[count] = new Edge();
				edges[count]
						.setSource(nodes[Integer.parseInt(cr.getValues()[0])]);
				edges[count]
						.setTarget(nodes[Integer.parseInt(cr.getValues()[1])]);
				count++;
			}
			fr.setArea(2000000, 2000000);

			ss = new ServerSocket(22222);
			sk = ss.accept();
			br = new BufferedReader(new InputStreamReader(sk.getInputStream()));
			PrintWriter pw = new PrintWriter(new OutputStreamWriter(
					sk.getOutputStream()));
			fr.setEdges(edges);
			fr.setNodes(nodes);
			while (true) {
				try {
					while (true) {
						String aci = br.readLine();
						// String aci = "init";
						System.out.println("2");
						if (aci == null) {
							// System.out.println("1");
							continue;
						}
						aci = aci.trim();
						os = sk.getOutputStream();
						aci = aci.trim();

						if (aci.equals("init")) {
							os.write(getint(nodes.length));
							os.write('\n');
							os.write(getint(edges.length));
							os.write('\n');
							for (Edge edge : edges) {
								os.write(getint(edge.getSource().id));
								os.write(getint(edge.getTarget().id));
								os.write('\n');
							}
						} else {
							// aci = br.readLine();
							String[] str = aci.split(",");
							int x = Integer.parseInt(str[0]);
							int y = Integer.parseInt(str[1]);
							int ceng = Integer.parseInt(str[2]);
							fr.setEdges(edges);
							fr.setNodes(nodes);
							fr.speed = 10000;
							List<Node> resultnode = new ArrayList<Node>();
							fr.getNode(ceng, x, y);
							for (Node node : fr.list) {
								fr.getCeng_node(resultnode, node, 0, 7);
							}
							for (Node node : resultnode) {
								os.write(getint(node.id));
								os.write(getint((int) node.x));
								os.write(getint((int) node.y));
								os.write(node.nodenum);
							}
							os.flush();
						}
					}
				} catch (IOException e) {
					e.printStackTrace();
					if (sk != null) {
						sk.close();
					}

					if (ss != null) {
						ss.close();
					}

					ss = new ServerSocket(22222);
					sk = ss.accept();
					br = new BufferedReader(new InputStreamReader(sk.getInputStream()));
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				System.out.println("good");
				sk.close();
				ss.close();
				os.close();
			} catch (IOException e) {
				e.printStackTrace();
			}

		}
	}

	public static byte[] putDouble(double x, int index) {
		byte[] bb = new byte[8];
		long l = Double.doubleToLongBits(x);
		for (int i = 0; i < 8; i++) {
			bb[index + i] = new Long(l).byteValue();
			l = l >> 8;
		}
		return bb;
	}

	public static byte[] getint(int u) {
		byte[] b = new byte[4];
		b[0] = (byte) (u);
		b[1] = (byte) (u >> 8);
		b[2] = (byte) (u >> 16);
		b[3] = (byte) (u >> 24);
		return b;
	}
}
