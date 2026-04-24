import subprocess
import unittest

class TestDatabase(unittest.TestCase):
    def run_commands(self, commands):
        process = subprocess.Popen(
            ["../build/omanidb"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            text=True
        )

        input = "\n".join(commands) + "\n"
        output, _ = process.communicate(input)
        raw_lines =  output.splitlines()
        clean_lines = [line for line in raw_lines if not line.startswith("Omani db > ")]
        return clean_lines

    def test_happy_path(self):
        commands = [
            "insert 1 ahmed@gmail.com 123",
            "select",
            ".exit"
        ]

        output = self.run_commands(commands)
        self.assertEqual(output[0], "Command excuted successfully.")
        self.assertEqual(output[1], "(1, ahmed@gmail.com, 123)")
        self.assertEqual(output[2], "Command excuted successfully.")
    
    def test_table_full(self):
        commands = []
        for i in range(1401):
            commands.append("insert 1 1@gmail.com 1")
        commands.append(".exit")
        
        output = self.run_commands(commands)
        print(len(output))

        self.assertEqual(output[-1], "Error: Table is full.")

    def test_email_buffer_overflow(self):
        long_email = "a" * 300 + "@gmail.com"

        commands = [
            f"insert 1 {long_email} 123",
            "select",
            ".exit"
        ]

        output = self.run_commands(commands)

        self.assertEqual(output[0], "Error: Email is too long.")
    
    def test_password_buffer_overflow(self):
        long_password = "a" * 25

        commands = [
            f"insert 1 1@gmail.com {long_password}",
            ".exit"
        ]

        output = self.run_commands(commands)
        self.assertEqual(output[0], "Error: Password is too long.")

if __name__ == '__main__':
    unittest.main()