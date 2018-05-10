import javax.swing.*;
import java.awt.Font;

public class Hello extends JFrame {

    public Hello(){
        JLabel l = new JLabel("Hello Java");
        l.setFont(new Font("Serif", Font.PLAIN, 24));
        add(l);
        setSize(200, 100);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
    }
}
