public class ResistorPattern {
    private int value;
    private ArrayList<ArrayList<Integer>> seriesList;
    private ArrayList<ArrayList<Integer>> parallelList;
    private int maxIndex;

    /*

    [S0 | S1 | S2 | ...] <- seriesList

    [P0 | P1 | P2 | ...] <- parallelList

    value = series(S2, parallel(P2, series(S1, parallel(P1, series(S0, parallel(P0, 0)))))
    */

    private float series(ArrayList<Integer> resistorsList, int next) {
        int sum = next;
        for (int i = 0; i < resistorsList.size(); i++) {
            sum += resistorsList.get(i);
        }
        return (float) sum;
    }

    private float parallel(ArrayList<Integer> resistorsList, int next) {
        float sum = 0;
        for (int i = 0; i < resistorsList.size(); i++) {
            sum += 1.0 / resistorsList.get(i);
        }
        sum += 1.0 / next;
        return 1 / sum;
    }

    private int computeValue(int index) {
        if (index == 0) {
            return series(seriesList[index], parallel(parallelList[index], 0));
        } else {
            return series(seriesList[index], parallel(parallelList[index], computeValue(index--)));
        }
    }

    private void computeValue() {
        value = computeValue(maxIndex);
    }

    public ResistorPatern () {
        value = 0;
        maxIndex = 0;
        seriesList = new ArrayList<ArrayList<Integer>>();
        parallelList = new ArrayList<ArrayList<Integer>>();
    }

    public ResistorPattern (ArrayList<ArrayList<Integer>> seriesList, ArrayList<ArrayList<Integer>> parallelList) {
        value = 0;
        assert(Math.abs(seriesList.size() - parallelList.size() <= 1);
        maxIndex = seriesList.size() > parallelList.size() ? seriesList.size() : parallelList.size();
        computeValue();
    }

    public void insert(int resistor, int index, boolean series) {
        assert(index - maxIndex <= 1);
        if (series) {
            seriesList.add(index, seriesList.get(index).add(reistor));
        } else {
            parallelList.add(index, parallelList.get(index).add(resistor));
        }
        if (index > maxIndex) {
            maxIndex = index;
        }
        computeValue();
    }

    public void insert(ArrayList<Integer> list, int index, boolean series) {
        assert(index - maxIndex <= 1);
        if (series) {
            seriesList.add(index, seriesList.get(index).addAll(list));
        } else {
            parallelList.add(index, parallelList.get(index).addAll(list));
        }
        if (index > maxIndex) {
            maxIndex = index;
        }
        computeValue();
    }

    public int getValue() {
        return value;
    }
}