import java.util.*;
import java.io.*;

public class ResistorProgram {
    private static ResistorGuide guide;
    private static ResistorCollection collection;

    private static void findResistorByValue(Scanner scanner) {
        System.out.print("Input a resistance in ohms: ");
        String resistanceStr = scanner.nextLine();
        try {
            int resistance = Integer.parseInt(resistanceStr);
            System.out.println(resistanceStr + " ohms: " + Arrays.toString(guide.getColor(resistance)));
        } catch(IllegalArgumentException e){
            System.out.println("Please provide a valid numerical value.");
        }
    }

    private static void findResistorByColor(Scanner scanner) {
        System.out.println("Please use the following keywords for colors (first letter capitalized): Black, Brown, Red, Orange, Yellow, Green, Blue, Violet, Grey, White");
        String[] colors = new String[3];
        System.out.print("First band color: ");
        colors[0] = scanner.nextLine();
        System.out.print("Second band color: ");
        colors[1] = scanner.nextLine();
        System.out.print("Multiplier: ");
        colors[2] = scanner.nextLine();
        System.out.println(Arrays.toString(colors) + " : " + Integer.toString(guide.getResistance(colors)) + " ohms.");
    }

    // TODO: include adding by color code
    private static void addResistor(Scanner scanner) {
        System.out.print("Please input a value: ");
        String resistanceStr = scanner.nextLine();
        try {
            int resistance = Integer.parseInt(resistanceStr);
            collection.add(resistance);
            System.out.println("Added!");
        } catch(IllegalArgumentException e) {
            System.out.println("Please provide a valid numerical value.");
        }
    }

    private static void removeResistor(Scanner scanner) {
        System.out.print("Please input a value: ");
        String resistanceStr = scanner.nextLine();
        try {
            int resistance = Integer.parseInt(resistanceStr);
            collection.remove(resistance);
            System.out.println("Removed!");
        } catch(IllegalArgumentException e) {
            System.out.println("Please provide a valid numerical value.");
        }
    }

    private static void viewCollection() {
        System.out.println();
        System.out.println(collection.toString());
    }

    /*
    get set of resistors
    while (! set.empty)
        remove(resistor)
        for (all other resistors in set)

    */
    private static void findResistorCombos(int target, int margin, int maxNumResistors) {
        ArrayList<Integer> resistorList = collection.getList();
        Queue<ResistorPattern> queue = new LinkedList<ResistorPattern>();
        int numResistors = 1;
        //for (int resistor: resistorList) {
        //    queue.add(new ArrayList<Integer>.add(resistor));
        //}
        ResistorPattern headList = queue.remove();
        while (!queue.isEmpty() && headList.size() < maxNumResistors) {
            // if series(headList, next) == target: break
            // queue.add(series(headList, next))
            // if parallel(headList, next) == target: break
            // queue.add(series(headList, next))
            headList = queue.remove();
        }
    }


    // temporary assumption: function won't be used for < 2 resistors
    private static void findResistorCombos(Scanner scanner) {

    }

    public static void main(String[] args) {
        String textFile = "resistors.txt";
        if (args.length > 2) {
            textFile = args[2];
        }
        try {
            collection = new ResistorCollection("resistors.txt");
            guide = new ResistorGuide();
            Scanner scanner = new Scanner(System.in);
            System.out.println("To exit at any point, type 'q' and hit ENTER");
            boolean quit = false;
            while (!quit) {
                System.out.println();
                System.out.println("a) look up a resistor color code by resistance value");
                System.out.println("b) look up a resistor value by its color code");
                System.out.println("c) add a resistor to your collection");
                System.out.println("d) remove a resistor from your collection");
                System.out.println("e) view your collection");
                System.out.println("f) use resistors from your collection to create a desired resistance");
                System.out.print("\nWhat would you like to do? ");
                switch(scanner.nextLine().toLowerCase()){
                    case "a": findResistorByValue(scanner); break;
                    case "b": findResistorByColor(scanner); break;
                    case "c": addResistor(scanner); break;
                    case "d": removeResistor(scanner); break;
                    case "e": viewCollection(); break;
                    case "f": findResistorCombos(scanner); break;
                    case "q": collection.save(); quit = true; break;
                   default: System.out.println("Sorry, that option isn't recognized..."); break;
                }
            }
        } catch(Exception e) {
            System.out.println("The text file " + textFile + " was not found or is in the wrong format. Terminating...");
        }
    }
    
}