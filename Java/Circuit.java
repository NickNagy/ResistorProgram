import java.util.*;

private class Edge {
    private float localResistance = 0.0;
    private Set<Integer> resistors;
    
    float nonLocalResistance = 0.0;

    private void computeLocalResistance() {
        float sum = 0.0;
        for (int r: resistors) {
            sum += (1.0 / r);
        }
        localResistance = 1.0 / sum;
    }

    public Edge () {
        resistors = new HashSet<Integer>();
    }

    public void insert(int r) {
        resistors.add(r);
        computeLocalResistance();
    }

    public int remove(int r) {
        if (resistors.contains(r)) {
            resistors.remove(r);
            computeLocalResistance();
            return 1;
        }
        return 0;
    }

    public float getLocalResistance() {
        return localResistance;
    }

    public float getTotalResistance() {
        if (nonLocalResistance == 0.0 && localResistance == 0.0) return 0.0; // simplify? gate-level?
        if (nonLocalResistance == 0.0) return localResistance;
        if (localResistance == 0.0) return nonLocalResistance;
        return nonLocalResistance*localResistance/(localResistance + nonLocalResistance); // careful of overflow
    }

    HashSet<Integer> getResistors() {
        Set<Integer> resistorsCopy = new HashSet<Integer>();
        for (int r : resistors) {
            resistorsCopy.add(r);
        }
        return resistorsCopy;
    }
}

public class CircuitMatrix {
    private unsigned int size = 2;
    private List<List<Integer>> matrix;
    
    public CircuitMatrix() {
        matrix = new ArrayList<ArrayList<Integer>>(size);
        for (int i = 0; i < size; i++) {
            matrix.add(new ArrayList<Integer>(2))
        }
        CircuitEdge nullEdge = new CircuitEdge();
        CircuitEdge n0n1 = new CircuitEdge();
        matrix[0][0] = nullEdge;
        matrix[1][1] = nullEdge;
        matrix[0][1] = n0n1;
        matrix[1][0] = n0n1;
    }
    
    private void resize() {
        size++;
    }

    private void refresh(int x, int y) {
        int i = x-1;
        while (i >= 0) {
            
        }
    }
}