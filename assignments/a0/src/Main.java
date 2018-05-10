import java.io.PrintWriter;

public class Main {

    public static void main(String[] args) {
        Hello hello = new Hello();
        try {
            PrintWriter writer = new PrintWriter("results.txt", "UTF-8");
            writer.println(System.getProperty("java.version"));
            writer.close();
        } catch (Exception ex) {
            System.out.println(ex.toString());
        }
    }
}
