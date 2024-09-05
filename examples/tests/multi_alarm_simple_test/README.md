# Test Multiple Alarms (Simple)

This tests the virtual alarms available to userspace. It sets two
repeating alarms, at 500ms and 1s respectively, each prints the alarm
index (1 or 2), current tick and alarm expiration to the console. When
successful, both alarms should fire and alarm 1 should fire twice as
often as alarm 2.
