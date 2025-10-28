
> by Rishav Sharma,  SSU ID **007747214**  > [Github Repo](https://github.com/notBjorn/P3_HopeForTheBest)

## **Collaboration & Sources**
This code and the logic used until the mapping section is mostly mine. Throughout this project I was really confused as to what i was supposed to do so i had to ask my peers some clarifying questions regarding the goals of the project. Will helped me in finding the issue with my input but besides that I had no other help. I did use the Clion inbuilt AI, but the purpose of that was just to help me find some random errors and warnings I was receiving which I will explain further in Implementation.
## **Implementation Details**
The code i implemented was heavily influenced from the labs we had done, especially the binary search tree lab.
Every single logic and coding aspect of this assignment I was prepared besides the maps. Labs were really useful as I knew what I had to do. The logic for building the code was pretty straight forward as I had already done exercises similar to this in class. I had to use AI to bring myself up to speed on maps and it seems that they are really useful as from what I understand there operations are constant time, at least the find operation. But besides this CLion's built-in AI assistant was used solely for identifying syntax errors and warning messages, not for algorithm design or logic implementation.


## **Testing & Status**
I think my code works to the best of my knowledge and testing. I did face a lot of issues when trying to test this code mainly because I was using leftover logic from part1 for the input file. I had other issues as well one major one was that I was unknowingly working remotely on blue through Clion and this caused me many headaches as I was trying to figure out why files were not being created on my local system. I changed a lot of code to figure this out but in the end the issue was that I was working on a remote server and looking for new files on my laptop.



```bash final output
rsharma@blue:~/P3_HopeForTheBest$ bash compile_and_test_project3_final_version.bash 

Compiling: g++ -std=c++20 -Wall BinSearchTree.cpp HuffmanTree.cpp main.cpp PriorityQueue.cpp Scanner.cpp utils.cpp -o p3_complete.x

Scanning for inputs in: input_output

  

==> Running: p3_complete.x on the_toil_of_trace_and_trail.txt

BST height: 27

BST unique words: 1432

Total tokens: 5471

Min frequency: 1

Max frequency: 371

Comparing: input_output/the_toil_of_trace_and_trail.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/the_toil_of_trace_and_trail.tokens

tokens matches

Comparing: input_output/the_toil_of_trace_and_trail.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/the_toil_of_trace_and_trail.freq

freq matches

Comparing: input_output/the_toil_of_trace_and_trail.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/the_toil_of_trace_and_trail.code

code matches

Comparing: input_output/the_toil_of_trace_and_trail.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/the_toil_of_trace_and_trail.hdr

hdr matches

  

==> Running: p3_complete.x on the_dominant_primordial_beast.txt

BST height: 24

BST unique words: 1436

Total tokens: 5255

Min frequency: 1

Max frequency: 386

Comparing: input_output/the_dominant_primordial_beast.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/the_dominant_primordial_beast.tokens

tokens matches

Comparing: input_output/the_dominant_primordial_beast.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/the_dominant_primordial_beast.freq

freq matches

Comparing: input_output/the_dominant_primordial_beast.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/the_dominant_primordial_beast.code

code matches

Comparing: input_output/the_dominant_primordial_beast.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/the_dominant_primordial_beast.hdr

hdr matches

  

==> Running: p3_complete.x on for_the_love_of_a_man.txt

BST height: 26

BST unique words: 1397

Total tokens: 4849

Min frequency: 1

Max frequency: 323

Comparing: input_output/for_the_love_of_a_man.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/for_the_love_of_a_man.tokens

tokens matches

Comparing: input_output/for_the_love_of_a_man.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/for_the_love_of_a_man.freq

freq matches

Comparing: input_output/for_the_love_of_a_man.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/for_the_love_of_a_man.code

code matches

Comparing: input_output/for_the_love_of_a_man.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/for_the_love_of_a_man.hdr

hdr matches

  

==> Running: p3_complete.x on who_has_won_to_mastership.txt

BST height: 23

BST unique words: 961

Total tokens: 3304

Min frequency: 1

Max frequency: 225

Comparing: input_output/who_has_won_to_mastership.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/who_has_won_to_mastership.tokens

tokens matches

Comparing: input_output/who_has_won_to_mastership.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/who_has_won_to_mastership.freq

freq matches

Comparing: input_output/who_has_won_to_mastership.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/who_has_won_to_mastership.code

code matches

Comparing: input_output/who_has_won_to_mastership.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/who_has_won_to_mastership.hdr

hdr matches

  

==> Running: p3_complete.x on the_law_of_club_and_fang.txt

BST height: 21

BST unique words: 1106

Total tokens: 3373

Min frequency: 1

Max frequency: 196

Comparing: input_output/the_law_of_club_and_fang.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/the_law_of_club_and_fang.tokens

tokens matches

Comparing: input_output/the_law_of_club_and_fang.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/the_law_of_club_and_fang.freq

freq matches

Comparing: input_output/the_law_of_club_and_fang.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/the_law_of_club_and_fang.code

code matches

Comparing: input_output/the_law_of_club_and_fang.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/the_law_of_club_and_fang.hdr

hdr matches

  

==> Running: p3_complete.x on into_the_primitive.txt

BST height: 23

BST unique words: 1201

Total tokens: 3822

Min frequency: 1

Max frequency: 275

Comparing: input_output/into_the_primitive.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/into_the_primitive.tokens

tokens matches

Comparing: input_output/into_the_primitive.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/into_the_primitive.freq

freq matches

Comparing: input_output/into_the_primitive.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/into_the_primitive.code

code matches

Comparing: input_output/into_the_primitive.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/into_the_primitive.hdr

hdr matches

  

==> Running: p3_complete.x on tableOfContents_TheCallOfWild.txt

BST height: 5

BST unique words: 8

Total tokens: 14

Min frequency: 1

Max frequency: 7

Comparing: input_output/tableOfContents_TheCallOfWild.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/tableOfContents_TheCallOfWild.tokens

tokens matches

Comparing: input_output/tableOfContents_TheCallOfWild.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/tableOfContents_TheCallOfWild.freq

freq matches

Comparing: input_output/tableOfContents_TheCallOfWild.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/tableOfContents_TheCallOfWild.code

code matches

Comparing: input_output/tableOfContents_TheCallOfWild.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/tableOfContents_TheCallOfWild.hdr

hdr matches

  

==> Running: p3_complete.x on the_sounding_of_the_call.txt

BST height: 27

BST unique words: 1654

Total tokens: 6299

Min frequency: 1

Max frequency: 498

Comparing: input_output/the_sounding_of_the_call.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/the_sounding_of_the_call.tokens

tokens matches

Comparing: input_output/the_sounding_of_the_call.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/the_sounding_of_the_call.freq

freq matches

Comparing: input_output/the_sounding_of_the_call.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/the_sounding_of_the_call.code

code matches

Comparing: input_output/the_sounding_of_the_call.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/the_sounding_of_the_call.hdr

hdr matches

  

==> Running: p3_complete.x on the_call_of_the_wild.txt

BST height: 29

BST unique words: 5102

Total tokens: 35484

Min frequency: 1

Max frequency: 2479

Comparing: input_output/the_call_of_the_wild.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/the_call_of_the_wild.tokens

tokens matches

Comparing: input_output/the_call_of_the_wild.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/the_call_of_the_wild.freq

freq matches

Comparing: input_output/the_call_of_the_wild.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/the_call_of_the_wild.code

code matches

Comparing: input_output/the_call_of_the_wild.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/the_call_of_the_wild.hdr

hdr matches

  

==> Running: p3_complete.x on lab08_input.txt

BST height: 6

BST unique words: 13

Total tokens: 17

Min frequency: 1

Max frequency: 3

Comparing: input_output/lab08_input.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/lab08_input.tokens

tokens matches

Comparing: input_output/lab08_input.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/lab08_input.freq

freq matches

Comparing: input_output/lab08_input.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/lab08_input.code

code matches

Comparing: input_output/lab08_input.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/lab08_input.hdr

hdr matches

  

==> Running: p3_complete.x on TheBells.txt

BST height: 12

BST unique words: 49

Total tokens: 83

Min frequency: 1

Max frequency: 11

Comparing: input_output/TheBells.tokens  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/tokens/TheBells.tokens

tokens matches

Comparing: input_output/TheBells.freq  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/freq/TheBells.freq

freq matches

Comparing: input_output/TheBells.code  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/code/TheBells.code

code matches

Comparing: input_output/TheBells.hdr  and  /home/faculty/kooshesh/cs315_fall2025/project3/final_version/hdr/TheBells.hdr

hdr matches

  

Summary: 44 match(es), 0 diff(s).
```