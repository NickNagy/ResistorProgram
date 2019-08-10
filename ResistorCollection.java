import java.util.*;
import java.io.*;

// TODO: load resistor file without overwriting it!

public class ResistorCollection {
    private Map<Integer, Integer> map;
    private static String textFile;

    private void load() throws FileNotFoundException, IOException{//throws FileNotFoundException, IOException{
        BufferedReader br = new BufferedReader(new FileReader(textFile));
        String line;
        while ((line = br.readLine())!=null) {
            String[] lineArr = line.split(":");
            map.put(Integer.parseInt(lineArr[0]), Integer.parseInt(lineArr[1]));
        }
    }

    public ResistorCollection (String textFile) throws FileNotFoundException, IOException{
        this.textFile = textFile;
        map = new TreeMap<Integer, Integer>();
    }

    public void add(int value) {
        if (map.containsKey(value)) {
            int numResistors = map.get(value);
            map.put(value, numResistors++);
        } else {
            map.put(value, 1);
        }
    }

    public void remove(int value) {
        if (map.containsKey(value)) {
            int numResistors = map.get(value);
            map.put(value, numResistors--);
        }
    }

    public void save() throws FileNotFoundException, UnsupportedEncodingException{
        PrintWriter writer = new PrintWriter(textFile, "UTF-8");
        for (int key : map.keySet()) {
            writer.println(Integer.toString(key) + ":" + Integer.toString(map.get(key)));
        }
        writer.close();
    }

    public String toString() {
        return "**placeholder**";
    }
}