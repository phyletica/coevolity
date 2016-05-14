#include "catch.hpp"
#include "ecoevolity/spreadsheet.hpp"

#include "ecoevolity/rng.hpp"
#include "ecoevolity/path.hpp"

RandomNumberGenerator _RNG = RandomNumberGenerator();

TEST_CASE("Testing parse_header on empty file", "[spreadsheet]") {
    std::string test_path = "data/tmp-" + _RNG.random_string(10) + ".txt";
    std::ofstream test_file;
    test_file.open(test_path);
    test_file.close();
    REQUIRE(path::exists(test_path));

    SECTION("Testing empty file") {
        std::ifstream in_stream;
        in_stream.open(test_path);
        std::vector<std::string> header;
        header = spreadsheet::parse_header(in_stream);
        in_stream.close();
        REQUIRE(header.size() == 0);
    }
}

TEST_CASE("Testing parse_header on empty stream", "[spreadsheet]") {
    SECTION("Testing empty stream") {
        std::stringstream stream;

        std::vector<std::string> header;
        header = spreadsheet::parse_header(stream);
        REQUIRE(header.size() == 0);
    }
}

TEST_CASE("Testing parse_header on simple header with three columns", "[spreadsheet]") {
    SECTION("Testing simple header only") {
        std::stringstream stream;
        stream << "col1\tcol2\tcol3\n";
        std::vector<std::string> expected = {"col1", "col2", "col3"};

        std::vector<std::string> header;
        header = spreadsheet::parse_header(stream);
        REQUIRE(header == expected);
    }

    SECTION("Testing simple header with data row") {
        std::stringstream stream;
        stream << "col1\tcol2\tcol3\n";
        stream << "1.0\t2.0\t3.0\n";
        std::vector<std::string> expected = {"col1", "col2", "col3"};

        std::vector<std::string> header;
        header = spreadsheet::parse_header(stream);
        REQUIRE(header == expected);
    }
}

TEST_CASE("Testing parse on empty file", "[spreadsheet]") {
    std::string test_path = "data/tmp-" + _RNG.random_string(10) + ".txt";
    std::ofstream test_file;
    test_file.open(test_path);
    test_file.close();
    REQUIRE(path::exists(test_path));

    SECTION("Testing stream from empty file") {
        std::ifstream in_stream;
        in_stream.open(test_path);

        std::map<std::string, std::vector<std::string> > data;
        REQUIRE_THROWS_AS(spreadsheet::parse(in_stream, data),
                EcoevolityParsingError);
        in_stream.close();
    }

    SECTION("Testing path of empty file") {
        std::map<std::string, std::vector<std::string> > data;
        REQUIRE_THROWS_AS(spreadsheet::parse(test_path, data),
                EcoevolityParsingError);
    }
}

TEST_CASE("Testing parse on empty stream", "[spreadsheet]") {
    SECTION("Testing empty stream") {
        std::stringstream stream;

        std::map<std::string, std::vector<std::string> > data;
        REQUIRE_THROWS_AS(spreadsheet::parse(stream, data),
                EcoevolityParsingError);
    }
}

TEST_CASE("Testing parse on simple header with three columns", "[spreadsheet]") {
    SECTION("Testing simple header only") {
        std::stringstream stream;
        stream << "col1\tcol2\tcol3\n";

        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(stream, data);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 0);
        REQUIRE(data.at("col2").size() == 0);
        REQUIRE(data.at("col3").size() == 0);
    }

    SECTION("Testing simple header with one data row") {
        std::stringstream stream;
        stream << "col1\tcol2\tcol3\n";
        stream << "1.0\t2.0\t3.0\n";

        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(stream, data);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 1);
        REQUIRE(data.at("col2").size() == 1);
        REQUIRE(data.at("col3").size() == 1);
        REQUIRE(data.at("col1").at(0) == "1.0");
        REQUIRE(data.at("col2").at(0) == "2.0");
        REQUIRE(data.at("col3").at(0) == "3.0");
    }
}

TEST_CASE("Testing parse offset", "[spreadsheet]") {
    std::string test_path = "data/tmp-" + _RNG.random_string(10) + ".txt";
    std::ofstream os;
    os.open(test_path);
    os << "col1\tcol2\tcol3\n";
    os << "11.0\t12.0\t13.0\n";
    os << "21.0\t22.0\t23.0\n";
    os << "31.0\t32.0\t33.0\n";
    os << "41.0\t42.0\t43.0\n";
    os << "51.0\t52.0\t53.0\n";
    os << "61.0\t62.0\t63.0\n";
    os << "71.0\t72.0\t73.0\n";
    os << "81.0\t82.0\t83.0\n";
    os << "91.0\t92.0\t93.0\n";
    os << "101.0\t102.0\t103.0\n";
    os.close();
    REQUIRE(path::exists(test_path));

    SECTION("Testing zero offset") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(test_path, data, 0);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 10);
        REQUIRE(data.at("col2").size() == 10);
        REQUIRE(data.at("col3").size() == 10);
        std::vector<std::string> c1 = {
            "11.0", "21.0", "31.0", "41.0", "51.0",
            "61.0", "71.0", "81.0", "91.0", "101.0"};
        std::vector<std::string> c2 = {
            "12.0", "22.0", "32.0", "42.0", "52.0",
            "62.0", "72.0", "82.0", "92.0", "102.0"};
        std::vector<std::string> c3 = {
            "13.0", "23.0", "33.0", "43.0", "53.0",
            "63.0", "73.0", "83.0", "93.0", "103.0"};
        REQUIRE(data.at("col1") == c1);
        REQUIRE(data.at("col2") == c2);
        REQUIRE(data.at("col3") == c3);
    }

    SECTION("Testing one offset") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(test_path, data, 1);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 9);
        REQUIRE(data.at("col2").size() == 9);
        REQUIRE(data.at("col3").size() == 9);
        std::vector<std::string> c1 = {
            "21.0", "31.0", "41.0", "51.0",
            "61.0", "71.0", "81.0", "91.0", "101.0"};
        std::vector<std::string> c2 = {
            "22.0", "32.0", "42.0", "52.0",
            "62.0", "72.0", "82.0", "92.0", "102.0"};
        std::vector<std::string> c3 = {
             "23.0", "33.0", "43.0", "53.0",
            "63.0", "73.0", "83.0", "93.0", "103.0"};
        REQUIRE(data.at("col1") == c1);
        REQUIRE(data.at("col2") == c2);
        REQUIRE(data.at("col3") == c3);
    }

    SECTION("Testing five offset") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(test_path, data, 5);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 5);
        REQUIRE(data.at("col2").size() == 5);
        REQUIRE(data.at("col3").size() == 5);
        std::vector<std::string> c1 = {
            "61.0", "71.0", "81.0", "91.0", "101.0"};
        std::vector<std::string> c2 = {
            "62.0", "72.0", "82.0", "92.0", "102.0"};
        std::vector<std::string> c3 = {
            "63.0", "73.0", "83.0", "93.0", "103.0"};
        REQUIRE(data.at("col1") == c1);
        REQUIRE(data.at("col2") == c2);
        REQUIRE(data.at("col3") == c3);
    }

    SECTION("Testing nine offset") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(test_path, data, 9);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 1);
        REQUIRE(data.at("col2").size() == 1);
        REQUIRE(data.at("col3").size() == 1);
        std::vector<std::string> c1 = {
            "101.0"};
        std::vector<std::string> c2 = {
            "102.0"};
        std::vector<std::string> c3 = {
            "103.0"};
        REQUIRE(data.at("col1") == c1);
        REQUIRE(data.at("col2") == c2);
        REQUIRE(data.at("col3") == c3);
    }

    SECTION("Testing offset equals number of data lines") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(test_path, data, 10);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 0);
        REQUIRE(data.at("col2").size() == 0);
        REQUIRE(data.at("col3").size() == 0);
    }

    SECTION("Testing offset exceeds number of data lines") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(test_path, data, 20);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 0);
        REQUIRE(data.at("col2").size() == 0);
        REQUIRE(data.at("col3").size() == 0);
    }
}

TEST_CASE("Testing parse multiple files", "[spreadsheet]") {
    std::string test_path1 = "data/tmp-" + _RNG.random_string(10) + ".txt";
    std::string test_path2 = "data/tmp-" + _RNG.random_string(10) + ".txt";
    std::string test_path3 = "data/tmp-" + _RNG.random_string(10) + ".txt";
    std::ofstream os;
    os.open(test_path1);
    os << "col1\tcol2\tcol3\n";
    os << "a11.0\ta12.0\ta13.0\n";
    os << "a21.0\ta22.0\ta23.0\n";
    os << "a31.0\ta32.0\ta33.0\n";
    os << "a41.0\ta42.0\ta43.0\n";
    os << "a51.0\ta52.0\ta53.0\n";
    os << "a61.0\ta62.0\ta63.0\n";
    os << "a71.0\ta72.0\ta73.0\n";
    os << "a81.0\ta82.0\ta83.0\n";
    os << "a91.0\ta92.0\ta93.0\n";
    os << "a101.0\ta102.0\ta103.0\n";
    os.close();
    os.open(test_path2);
    os << "col1\tcol2\tcol3\n";
    os << "b11.0\tb12.0\tb13.0\n";
    os << "b21.0\tb22.0\tb23.0\n";
    os << "b31.0\tb32.0\tb33.0\n";
    os << "b41.0\tb42.0\tb43.0\n";
    os << "b51.0\tb52.0\tb53.0\n";
    os << "b61.0\tb62.0\tb63.0\n";
    os << "b71.0\tb72.0\tb73.0\n";
    os << "b81.0\tb82.0\tb83.0\n";
    os << "b91.0\tb92.0\tb93.0\n";
    os << "b101.0\tb102.0\tb103.0\n";
    os.close();
    os.open(test_path3);
    os << "col1\tcol2\tcol3\n";
    os << "c11.0\tc12.0\tc13.0\n";
    os << "c21.0\tc22.0\tc23.0\n";
    os << "c31.0\tc32.0\tc33.0\n";
    os << "c41.0\tc42.0\tc43.0\n";
    os << "c51.0\tc52.0\tc53.0\n";
    os << "c61.0\tc62.0\tc63.0\n";
    os << "c71.0\tc72.0\tc73.0\n";
    os << "c81.0\tc82.0\tc83.0\n";
    os << "c91.0\tc92.0\tc93.0\n";
    os << "c101.0\tc102.0\tc103.0\n";
    os.close();
    REQUIRE(path::exists(test_path1));
    REQUIRE(path::exists(test_path2));
    REQUIRE(path::exists(test_path3));
    std::vector<std::string> paths = {test_path1, test_path2, test_path3};

    SECTION("Testing zero offset") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(paths, data, 0);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 30);
        REQUIRE(data.at("col2").size() == 30);
        REQUIRE(data.at("col3").size() == 30);
        std::vector<std::string> c1 = {
            "a11.0", "a21.0", "a31.0", "a41.0", "a51.0",
            "a61.0", "a71.0", "a81.0", "a91.0", "a101.0",
            "b11.0", "b21.0", "b31.0", "b41.0", "b51.0",
            "b61.0", "b71.0", "b81.0", "b91.0", "b101.0",
            "c11.0", "c21.0", "c31.0", "c41.0", "c51.0",
            "c61.0", "c71.0", "c81.0", "c91.0", "c101.0"
        };
        std::vector<std::string> c2 = {
            "a12.0", "a22.0", "a32.0", "a42.0", "a52.0",
            "a62.0", "a72.0", "a82.0", "a92.0", "a102.0",
            "b12.0", "b22.0", "b32.0", "b42.0", "b52.0",
            "b62.0", "b72.0", "b82.0", "b92.0", "b102.0",
            "c12.0", "c22.0", "c32.0", "c42.0", "c52.0",
            "c62.0", "c72.0", "c82.0", "c92.0", "c102.0"
        };
        std::vector<std::string> c3 = {
            "a13.0", "a23.0", "a33.0", "a43.0", "a53.0",
            "a63.0", "a73.0", "a83.0", "a93.0", "a103.0",
            "b13.0", "b23.0", "b33.0", "b43.0", "b53.0",
            "b63.0", "b73.0", "b83.0", "b93.0", "b103.0",
            "c13.0", "c23.0", "c33.0", "c43.0", "c53.0",
            "c63.0", "c73.0", "c83.0", "c93.0", "c103.0"
        };
        REQUIRE(data.at("col1") == c1);
        REQUIRE(data.at("col2") == c2);
        REQUIRE(data.at("col3") == c3);
    }

    SECTION("Testing two offset") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(paths, data, 2);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 24);
        REQUIRE(data.at("col2").size() == 24);
        REQUIRE(data.at("col3").size() == 24);
        std::vector<std::string> c1 = {
                              "a31.0", "a41.0", "a51.0",
            "a61.0", "a71.0", "a81.0", "a91.0", "a101.0",
                              "b31.0", "b41.0", "b51.0",
            "b61.0", "b71.0", "b81.0", "b91.0", "b101.0",
                              "c31.0", "c41.0", "c51.0",
            "c61.0", "c71.0", "c81.0", "c91.0", "c101.0"
        };
        std::vector<std::string> c2 = {
                              "a32.0", "a42.0", "a52.0",
            "a62.0", "a72.0", "a82.0", "a92.0", "a102.0",
                              "b32.0", "b42.0", "b52.0",
            "b62.0", "b72.0", "b82.0", "b92.0", "b102.0",
                              "c32.0", "c42.0", "c52.0",
            "c62.0", "c72.0", "c82.0", "c92.0", "c102.0"
        };
        std::vector<std::string> c3 = {
                              "a33.0", "a43.0", "a53.0",
            "a63.0", "a73.0", "a83.0", "a93.0", "a103.0",
                              "b33.0", "b43.0", "b53.0",
            "b63.0", "b73.0", "b83.0", "b93.0", "b103.0",
                              "c33.0", "c43.0", "c53.0",
            "c63.0", "c73.0", "c83.0", "c93.0", "c103.0"
        };
        REQUIRE(data.at("col1") == c1);
        REQUIRE(data.at("col2") == c2);
        REQUIRE(data.at("col3") == c3);
    }

    SECTION("Testing seven offset") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(paths, data, 7);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 9);
        REQUIRE(data.at("col2").size() == 9);
        REQUIRE(data.at("col3").size() == 9);
        std::vector<std::string> c1 = {
            "a81.0", "a91.0", "a101.0",
            "b81.0", "b91.0", "b101.0",
            "c81.0", "c91.0", "c101.0"
        };
        std::vector<std::string> c2 = {
            "a82.0", "a92.0", "a102.0",
            "b82.0", "b92.0", "b102.0",
            "c82.0", "c92.0", "c102.0"
        };
        std::vector<std::string> c3 = {
            "a83.0", "a93.0", "a103.0",
            "b83.0", "b93.0", "b103.0",
            "c83.0", "c93.0", "c103.0"
        };
        REQUIRE(data.at("col1") == c1);
        REQUIRE(data.at("col2") == c2);
        REQUIRE(data.at("col3") == c3);
    }

    SECTION("Testing offset equals number of lines per file") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(paths, data, 10);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 0);
        REQUIRE(data.at("col2").size() == 0);
        REQUIRE(data.at("col3").size() == 0);
    }

    SECTION("Testing offset exceeds number of lines per file") {
        std::map<std::string, std::vector<std::string> > data;
        spreadsheet::parse(paths, data, 11);

        REQUIRE(data.size() == 3);
        REQUIRE(data.at("col1").size() == 0);
        REQUIRE(data.at("col2").size() == 0);
        REQUIRE(data.at("col3").size() == 0);
    }
}
