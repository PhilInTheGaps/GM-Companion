package application;

public class Test {
	//TEST SORT
	public static void bubbleSort(){
		System.out.println("BubbleSort:");
		int[] a = {0, 8, 1, 2, 5, 3};
		for(int i : a){
			System.out.print(i+", ");
		}
		Boolean swapped = true;
		while (swapped){
			swapped = false;
			for (int i = 0; i<a.length-1; i++){
				if (a[i]>a[i+1]){
					int i1 = a[i];
					int i2 = a[i+1];
					a[i] = i2;
					a[i+1] = i1;
					swapped = true;
				}
			}
		}
		System.out.println("");
		for(int i : a){
			System.out.print(i+", ");
		}
		System.out.println("");
	}
	public static void selectSort(){
		int[] a = {0, 8, 1, 2, 5, 3, 12, 98, 43, 18, 1002, 3349, 432, 12, 1, 1, 1, 1, 982, 42, 430, 34};
		System.out.println("Selection Sort:");
		for (int i : a){
			System.out.print(i+", ");
		}
		System.out.println("");
		
		for (int i = 0; i < a.length - 1; i++) {
			for (int j = i + 1; j < a.length; j++) {
				if (a[i] > a[j]) {
					int temp = a[i];
					a[i] = a[j];
					a[j] = temp;
				}
			}
		}
		
		for (int i : a){
			System.out.print(i+", ");
		}
	}
	
	public static void caesar(){
		String text = "Hallo Welt. Das hier ist ein unverschluesselter Text";
		char[] a = text.toCharArray();
		char[] aEncrypted = new char[a.length];
		String abc = "abcdefghijklmnopqrstuvwxyz,.-_+*#'!?1234567890/&%:;| 1234567890";
		for(int i = 0; i<a.length; i++){
			int j = abc.indexOf(a[i]);
			char temp = abc.charAt(j+3);
			aEncrypted[i] = temp;
		}
		System.out.println("");
		System.out.println("Caesar Encryption:");
		for(char c : a){
			System.out.print(c);
		}
		System.out.println("");
		for(char c : aEncrypted){
			System.out.print(c);
		}
	} 
}
