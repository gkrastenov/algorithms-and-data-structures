using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Net.Http;
using Newtonsoft.Json;

namespace MerkleTreeImpelemtation
{
    class Program
    {
        private const string blockchainApi = "https://blockchain.info/rawblock/000000000000000000155a0c59bfdb54834608e7bf55e29920fd24591f1e3a98";
       
        static void Main(string[] args)
        {
            dynamic blockData = FetchBlockData(blockchainApi);
            if (blockData == null)
                throw new InvalidOperationException("response failed");
          
            var transactionHashes = new List<string>();
            foreach (dynamic transaction in blockData.tx)
            {
                transactionHashes.Add(SwapAndReverse(transaction.hash));
            }

            Console.WriteLine("Block Merkel Root: " + blockData.mrkl_root);
            Console.WriteLine("Block Header Hash: " + blockData.hash);
            Console.WriteLine("Nonce: " + blockData.nonce);

            MerkleTree merkleTree = new MerkleTree();
            string merkleRoot = merkleTree.BuildMerkleRoot(transactionHashes);
            Console.WriteLine("Computed Merkel Root: " + SwapAndReverse(merkleRoot).ToLower());
        }

        private static dynamic FetchBlockData(string path)
        {
            using (HttpClient client = new HttpClient())
            {
                HttpResponseMessage response = client.GetAsync(path).Result;
                if (response.IsSuccessStatusCode)
                {
                    var json = response.Content.ReadAsStringAsync().Result;
                    return JsonConvert.DeserializeObject<ExpandoObject>(json);
                }
            }
            return null;
        }

        private static string SwapAndReverse(string input)
        {
            string newString = string.Empty; ;
            for (int i = 0; i < input.Count(); i += 2)
            {
                newString += string.Concat(input[i + 1], input[i]);
            }
            return ReverseString(newString);
        }

        private static string ReverseString(string original)
        {
            return new string(original.Reverse().ToArray());
        }
    }
}