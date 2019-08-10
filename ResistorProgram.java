import java.util.*;

public class ResistorProgram {
    private static ResistorGuide guide;

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

    private static void addResistor() {

    }

    private static void removeResistor() {

    }

    private static void viewCollection() {

    }

    private static void findResistorCombos() {

    }

    public static void main(String[] args) {
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
            System.out.print("What would you like to do? ");
            switch(scanner.nextLine().toLowerCase()){
                case "a": findResistorByValue(scanner); break;
                case "b": findResistorByColor(scanner); break;
                case "c": addResistor(); break;
                case "d": removeResistor(); break;
                case "e": viewCollection(); break;
                case "f": findResistorCombos(); break;
                case "q": quit = true; break;
                default: System.out.println("Sorry, that option isn't recognized..."); break;
            }
        }
    }
    
}