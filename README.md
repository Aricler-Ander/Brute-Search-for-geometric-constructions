# Brute Search for geometric constructions
A solution searching program for compass and straightedge construction problems. Able to find a solution for most of levels in the game Euclidea. Also support compass-only and straightedge-only mode. Highly accessible. Top efficiency.

## Input
When you start the exe, you need to estimate the total steps needed and input it as a single number. **When counting the step limit, no need to include the initial elements that you input into the program.** No characters other than numbers are accepted.

Then choose a tool mode. You sometimes need to input extra random points to ensure straightedge-only mode can really construct something.

Input initial elements.  
**Format:**  
P(Point):only x,y in (x,y);  
L(Line):only a,b,c in ax+by=c;  
R(Ray):start point and a point on the ray, 4 numbers in total;  
S(Segment):2 end points, also 4 numbers in total;  
C(Circle):only a,b,r in (x-a)^2+(y-b)^2=r^2;  
**Only accept decimal numbers with at least 8 decimal places.**  
**When you input circles, also input circle centres if you need.**  
**No need to input the intersect points of these elements.**  

## Searching
After all your data is entered, the exe can start searching. **It may take a huge amount of time if you try to search for problems more than 7E (or s-only more than 11E).** The tool will provide the first solution it finds and then exit.
