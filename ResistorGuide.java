import java.util.*;

public class ResistorGuide {
    private static String colorMap[];

    private int colorToNumber(String color) {
        switch(color) {
            case "Black" : return 0;
            case "Brown" : return 1;
            case "Red"   : return 2;
            case "Orange": return 3;
            case "Yellow": return 4;
            case "Green" : return 5;
            case "Blue"  : return 6;
            case "Violet": return 7;
            case "Grey"  : return 8;
            case "White" : return 9;
            default:
                System.out.println("ERROR: invalid color argument");
                return -1;
        }
    }

    public ResistorGuide(){
        System.out.println("NOTE: This guide only supports 3-band resistors.");
        colorMap = new String[] {"Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Grey", "White"};
    }

    public String[] getColor(int resistance) {
        String colors[] = new String[3];
        int multiplier = 1;
        while (resistance / multiplier >= 100) { // want 2 digits
            multiplier *= 10;
        }
        if (resistance % multiplier != 0) {
            int removed = resistance % multiplier;
            System.out.println("WARNING: simplifying " + Integer.toString(resistance) + " to " + Integer.toString(resistance - removed));
            resistance -= removed;
        }
        int firstBand = resistance / (multiplier*10);
        int secondBand = (resistance % (multiplier*10)) / multiplier;
        colors[0] = colorMap[firstBand];
        colors[1] = colorMap[secondBand];
        colors[2] = colorMap[multiplier];
        return colors;
        //return new StringBuilder(colorMap[firstBand]).append(colorMap[secondBand]).append(colorMap[multiplier]).toString();
    }

    public int getResistance(String[] colors){
        assert (colors.length == 3);
        int resistance = colorToNumber(colors[0])*10;
        resistance += colorToNumber(colors[1]);
        return resistance * colorToNumber(colors[2]);
    }

    public String toString() {
        return "";
    }
}