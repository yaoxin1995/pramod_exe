



void main() 
{
	int choice, key, value, n, c, i;

	do {
		printf("Implementation of Hash Table in C chaining with Singly Linked List \n\n");
		printf("MENU-: \n1.Inserting item in the Hash Table"
                              "\n2.Removing item from the Hash Table"
                              "\n3.Check the size of Hash Table" 
                              "\n4.To display a Hash Table"
		       "\n\n Please enter your choice -: ");
 
 		scanf("%d", &choice);
 
		switch(choice) 
                {
 
		case 1:
 
		      printf("Inserting element in Hash Table\n");
		      printf("Enter key and value-:\t");
		      scanf("%d %d", &key, &value);
		      //insert(key, value);
 
		      break;
 
		case 2:
 
		      printf("Deleting in Hash Table \nEnter the key to delete-:");
		      scanf("%d", &key);
		      //remove_element(key);
 
		      break;
 
		case 3:
 
		      n = size_of_array();
		      printf("Size of Hash Table is-:%d\n", n);
 
		      break;
 
		case 4:
 
		      //display();
 
		      break;
 
		default:
 
		       printf("Wrong Input\n");
 
		}
 
		printf("\nDo you want to continue-:(press 1 for yes)\t");
		scanf("%d", &c);
 
	}while(c == 1);
 
	getchar();

}