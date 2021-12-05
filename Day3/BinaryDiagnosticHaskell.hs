import System.Directory.Internal.Prelude ( getArgs )
import Data.Char ( digitToInt )

main :: IO ()
main = do
    args <- getArgs
    content <- readFile (head args)
    let linesOfFiles = lines content
    powerConsumption linesOfFiles

countOnes :: [String] -> Int -> Int
countOnes [] _ = 0
countOnes (x:xs) p
    | (x !! p) == '1' = 1 + countOnes xs p
    | otherwise       = countOnes xs p

commonBit :: [String] -> Int -> Char
commonBit xs p
    | countOnes xs p > div (length xs) 2                     = '1'
    | countOnes xs p == div (length xs) 2 && even (length xs) = '1'
    | otherwise                                              = '0'

hasBit :: String -> Int -> Char -> Bool
hasBit x p c = (x !! p) == c

elimNumsAtPos :: [String] -> Int -> Bool -> [String]
-- elimNums xs p = filter (hasBit p (commonBit xs p)) xs
elimNumsAtPos xs p b = filter (\n -> (hasBit n p (commonBit xs p)) == b) xs

elimNums :: [String] -> Int -> Int -> Bool -> [String]
elimNums xs p m b
    | p < m && length xs > 1 = elimNums (elimNumsAtPos xs p b) (p + 1) m b
    | otherwise              = xs

toDec :: String -> Int 
toDec = foldl (\acc x -> acc * 2 + digitToInt x) 0

oxygenGenRating :: [String] -> Int
oxygenGenRating xs = toDec (head (elimNums xs 0 (length (head xs)) True))

co2ScrubberRating :: [String] -> Int
co2ScrubberRating xs = toDec (head (elimNums xs 0 (length (head xs)) False))

powerConsumption :: [String] -> IO ()
powerConsumption xs = putStrLn ("Power Consumption: " ++ show (oxygenGenRating xs * co2ScrubberRating xs))

