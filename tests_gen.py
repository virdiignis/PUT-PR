[print("""
omp_set_num_threads({});
t.start();
result = parallel_{}(2, MAX);
t.stop();
fprintf(f, "'parallel_{}', {}, %f\\n", t.get());
printf("'parallel_{}', {}, %f\\n", t.get());
""".format(th, fn, fn, th, fn, th)) for fn in range(1, 18) for th in (1, 3, 6, 12)]
