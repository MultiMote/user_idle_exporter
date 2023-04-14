# user_idle_exporter

[Prometheus](https://prometheus.io) Windows user idle time exporter. Must be run as user, not as system account.
Idle time is a number of seconds since last user interaction (keyboard/mouse).

Example output:

```
# HELP user_exporter_build_info User exporter build info
# TYPE user_exporter_build_info gauge
user_idle_exporter_build_info{version="0.1"} 1
# HELP user_idle_time User idle time
# TYPE user_idle_time gauge
user_idle_seconds 12
# HELP user_idle_error User idle time retrieval error
# TYPE user_idle_error gauge
user_idle_error 0
```

Default listen address (0.0.0.0) and port (9183) can be only changed in CMakeLists.txt.