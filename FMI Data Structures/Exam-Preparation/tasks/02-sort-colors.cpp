class Solution {
public:
    void sortColors(vector<int>& nums) {
    short zero, one, two, index;
	zero = one = two = index =0;
	for (size_t i = 0; i < nums.size(); i++)
	{
		if (nums[i] == 0){
			zero++;
		}else if (nums[i] == 1){
			++one;
		}else if (nums[i] == 2){
			++two;
		}
	}
	while (index < nums.size())
	{
		if (zero> 0){
			nums[index] = 0;
			--zero;
		}else if (one > 0){
			nums[index] = 1;
			--one;
		}
		else if (two > 0){
			nums[index] = 2;
			--two;
		}
		++index
	}  
    }
};