using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;

namespace MerkleTreeImpelemtation
{
    public class MerkleTree
    {
        public string BuildMerkleRoot(IList<string> merkelLeaves)
        {
            if (merkelLeaves == null || !merkelLeaves.Any())
                return string.Empty;

            if (merkelLeaves.Count() == 1)
                return merkelLeaves.First();

            if (merkelLeaves.Count() % 2 > 0)
                merkelLeaves.Add(merkelLeaves.Last());

            var merkleBranches = new List<string>();
            for (int i = 0; i < merkelLeaves.Count(); i += 2)
            {
                var leafPair = string.Concat(merkelLeaves[i], merkelLeaves[i + 1]);
                //double hash
                merkleBranches.Add(HashUsingSHA256(HashUsingSHA256(leafPair)));
            }
            return BuildMerkleRoot(merkleBranches);
        }

        private static string HashUsingSHA256(string data)
        {
            using (var sha256 = SHA256Managed.Create())
            {
                return ComputeHash(sha256, HexToByteArray(data));
            }
        }

        private static string ComputeHash(HashAlgorithm hashAlgorithm, byte[] input)
        {
            byte[] data = hashAlgorithm.ComputeHash(input);
            return ByteArrayToHex(data);
        }

        private static string ByteArrayToHex(byte[] bytes)
        {
            return BitConverter.ToString(bytes).Replace("-", "");
        }

        private static byte[] HexToByteArray(string hex)
        {
            return Enumerable.Range(0, hex.Length)
                .Where(x => x % 2 == 0)
                .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                .ToArray();
        }
    }
}