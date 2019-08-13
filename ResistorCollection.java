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
        load();
    }

    public void add(int value) {
        if (map.containsKey(value)) {
            int numResistors = map.get(value);
            map.put(value, numResistors+1);
        } else {
            map.put(value, 1);
        }
    }

    public void remove(int value) {
        if (map.containsKey(value)) {
            int numResistors = map.get(value);
            map.put(value, numResistors-1);
        }
    }

    public void save() throws FileNotFoundException, UnsupportedEncodingException{
        PrintWriter writer = new PrintWriter(textFile, "UTF-8");
        for (int key : map.keySet()) {
            writer.println(Integer.toString(key) + ":" + Integer.toString(map.get(key)));
        }
        writer.close();
    }

    public ArrayList<Integer> getList () {
        ArrayList<Integer> list = new ArrayList<Integer>();
        for (int key: map.keySet()) {
            for (int i = 0; i < map.get(key); i++) {
                list.add(key);
            }
        }
        return list;
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int key : map.keySet()) {
            sb.append(Integer.toString(key)).append(":").append(map.get(key).toString()).append("\n");
        }
        return sb.toString();
    }
}